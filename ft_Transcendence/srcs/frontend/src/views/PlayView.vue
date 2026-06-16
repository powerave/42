<script setup lang="ts">
import { onMounted, onUnmounted, ref, computed, watch } from 'vue'; 
import { onBeforeRouteLeave, useRoute, useRouter } from 'vue-router'; 
import { useAuthStore } from '../stores/auth';
import { getSocket, subscribe } from '../utils/socket'; 
import { fetchWithAuth } from '../utils/fetchWithAuth'; 
import CardView from '../components/game/CardView.vue';
import { useI18n } from 'vue-i18n';

const route = useRoute(); 
const router = useRouter(); 
const authStore = useAuthStore();
const { t } = useI18n();
const currentGameId = ref<number>(Number(route.params.id)); 
const gameState = ref<any>(null);
const myPlayerId = ref<number | null>(null);
const myHand = ref<any[]>([]);
const showColorPicker = ref<boolean>(false);
const lastActionEffect = ref<any>(null);
const gameWinner = ref<number | null>(null);
const playersAvatars = ref<Record<number, string>>({});
const showGameOverModal = ref<boolean>(false);
const gameOverData = ref<any>(null);
const sortByColor = ref<boolean>(true);
const cannotPlayMessage = ref<string>('');
const gameFinished = ref<boolean>(false);

function getPlayerName(player: any): string {
  if (!player) return '';
  if (player.username) return player.username;
  return `🤖 Bot ${player.player_number}`;
}

const myPlayerNumber = computed(() => {
  if (!gameState.value || !gameState.value.players || myPlayerId.value === null) return null;
  const me = gameState.value.players.find((p: any) => p.id === myPlayerId.value);
  return me ? me.player_number : null;
});

const isMyTurn = computed(() => {
  console.log('current_turn:', gameState.value?.current_turn);
  console.log('myPlayerId:', myPlayerId.value);
  console.log('myPlayerNumber:', myPlayerNumber.value);
  return gameState.value && gameState.value.current_turn === myPlayerNumber.value;
});

// Sort hand with original indices preserved
const sortedHandWithIndices = computed(() => {
  const hand = myHand.value.map((card, idx) => ({ card, originalIndex: idx }));
  if (sortByColor) {
    const colorOrder = { 'Red': 0, 'Green': 1, 'Yellow': 2, 'Blue': 3, 'Wild': 4 };
    hand.sort((a, b) => {
      const colorDiff = ((colorOrder as any)[a.card.colour] ?? 5) - ((colorOrder as any)[b.card.colour] ?? 5);
      if (colorDiff !== 0) return colorDiff;
      // Then sort by numeric value if possible
      const aVal = parseInt(a.card.value) || 999; // 999 for non-numeric values (Skip, Reverse, Draw Two, Wild)
      const bVal = parseInt(b.card.value) || 999;
      return aVal - bVal;
    });
  }
  return hand;
});

onBeforeRouteLeave((to, from, next) => {
    if (isReloading.value || gameFinished.value) {
      next();
      return;
    }
    const confirm = window.confirm(t('play.quit_confirm'));
    if (!confirm) {
      next(false); // annule la navigation
      return;
    }
    // Quitter proprement
    sessionStorage.removeItem('uno_session');
    getSocket()?.emit("message", {
      type: "LEAVE_GAME",
      game_id: currentGameId.value,
      player_id: myPlayerId.value
    });
  next();
});

// Determine which cards are playable
const playableIndices = computed(() => {
  if (!gameState.value || !isMyTurn.value) return new Set();
  
  const playable = new Set<number>();
  const currentColor = gameState.value.current_colour;
  const currentValue = gameState.value.current_value;
  
  myHand.value.forEach((card, index) => {
    if (card.colour === 'Wild') {
      // Wild cards are always playable
      playable.add(index);
    } else if (card.colour === currentColor) {
      // Cards matching the color are playable
      playable.add(index);
    } else if (card.value === currentValue) {
      // Cards matching the value are playable
      playable.add(index);
    }
  });
  
  return playable;
});

// Helper to get opponent info based on offset from my player number
function getOpponentByOffset(offset: number) {
  if (!gameState.value || !gameState.value.players || myPlayerNumber.value === null) return null;
  const N = gameState.value.players.length;
  if (offset >= N) return null;
  return gameState.value.players.find((p: any) => (p.player_number - myPlayerNumber.value + N) % N === offset);
}

const oppTop = computed(() => getOpponentByOffset(2));
const oppRight = computed(() => getOpponentByOffset(1));
const oppLeft = computed(() => getOpponentByOffset(3));

watch(() => gameState.value?.players, async (players) => {
  if (!players) return;
  // for (const p of players) {
  //   if (p.player_id && !playersAvatars.value[p.player_id]) {
  //     try {
  //       // WARN : cett erequette n'existe plus, il faut utiliser game_update pour les avatars

  //       const response = await authStore.authFetch(`/api/game/${p.player_id}`);
  //       const data = await response.json();
  //       if (data.avatarUrl) {
  //        playersAvatars.value[p.player_id] = data.avatarUrl;
  //       }
  //     } catch (e) {
  //       console.error("Failed to fetch avatar", e);
  //     }
  //   }
  //}
}, { deep: true, immediate: true });

const unsub: Array<() => void> = [];
const isReloading = ref(false);

function handleBeforeUnload(e: BeforeUnloadEvent) {
  if (!gameFinished.value)
    isReloading.value = true;
}

// handle browser refresh or close to prevent losing the game without confirmation
onMounted(() => {
  window.addEventListener('beforeunload', handleBeforeUnload);
  if (!authStore.user) {
    // if user data is not yet loaded, wait for it before initializing the game connection and trying to reconnect
    const unwatch = watch(() => authStore.user, (newUser) => {
      if (newUser) {
        initGameConnection();
        tryReconnect();
        unwatch();
      }
      console.log('authStore.user changed:', newUser);
    });
  } else {
    // if user data is already available, initialize the game connection immediately
    console.log('User already in store:', authStore.user);
    initGameConnection();
    tryReconnect();
  }
});

// Clean up on unmount
onUnmounted(() => { 
  unsub.forEach(u => u());
  window.removeEventListener('beforeunload', handleBeforeUnload);
});

function initGameConnection() {
  const socketInstance = getSocket();
  if (!socketInstance) {
    //console.error("Socket instance is not available");
    return;
  }
  unsub.push(subscribe('connect', (data: any) => {
    console.log('Connecté au serveur de jeu, socket id :', data.id);
  }));

  unsub.push(subscribe('disconnect', () => {
    console.log('Déconnecté du serveur de jeu');
  }));

  unsub.push(subscribe('connect_error', (err: any) => {
    //console.error('Erreur de connexion :', err?.message || err);
  }));

  unsub.push(subscribe('message', (data: any) => {
    console.log('Message reçu :', data.type, data);
  
    switch (data.type) {
      case 'GAME_UPDATE':
        gameState.value = data;
        // //const currentUserId = authStore.user?.id;
        // //if (currentUserId) {
        // const me = data.players?.find((p: any) => p.id === myPlayerId.value);
        // if (me) {
        //   myPlayerId.value = me.player_number;
        // }
        // //}
        break;

      case 'HAND_UPDATE':
        myHand.value = data.hand;
        myPlayerId.value = data.player_id;

        if (data.game_id) {
          sessionStorage.setItem('uno_session', JSON.stringify({
            gameId:   data.game_id,
            playerId: data.player_id,
            //userId:   authStore.user?.id
          }));
        }

        if (data.game_id) {
          sessionStorage.setItem('uno_session', JSON.stringify({
            gameId:   data.game_id,
            playerId: data.player_id,
            //userId:   authStore.user?.id
          }));
        }

        lastActionEffect.value = data.effect ? {
          type: data.effect,
          drewPlayer: data.drew_player,
          drewCount: data.drew_count
        } : null;
        break;

      case 'CHOOSE_COLOUR':
        if (data.player_id === myPlayerId.value) {
          showColorPicker.value = true;
        }
        break;

      case 'GAME_OVER':
        sessionStorage.removeItem('uno_session');
        gameFinished.value = true;
        gameWinner.value = data.winner_player_number;
        gameOverData.value = data;
        showGameOverModal.value = true;
        
        // Récupérer l'XP actualisée après la partie
        fetchWithAuth('/api/home/refresh_xp')
          .then(res => res.json())
          .then(xpData => {
            if (authStore.user) {
               authStore.user.xp = xpData.user_xp;
            }
          })
          .catch(e => /*console.error('Erreur lors du rafraîchissement de l\'XP:', e)*/ null);
          authStore.checkSession();
        break;

      case 'ERROR':
        //console.error('Action rejetée :', data.message);
        alert(`Erreur: ${data.message}`);
        break;

      case 'RECONNECT_DENIED':
        sessionStorage.removeItem('uno_session');
        router.push('/');
        break;
    }
  }));

  //if (socketInstance.connected) {
  //    joinGame();
  //  }
}

function tryReconnect() {
  const raw = sessionStorage.getItem('uno_session');
  if (!raw) return;

  const session = JSON.parse(raw);

  if (session.gameId !== currentGameId.value) {
    sessionStorage.removeItem('uno_session');
    return;
  }

  console.log('Session trouvée, tentative de reconnexion…');
  getSocket()?.emit('message', {
    type:    'RECONNECT',
    game_id: session.gameId,
  });
}

/*onUnmounted(() => {
  if (socket.value) {
    socket.value.disconnect();
  }
});*/

// function joinGame(gameId: number) {
  
//   const socket = getSocket();
//   if (socket) {
//     console.log(`Tentative de rejoindre la partie ${gameId}`);
//     socket.emit("message", {
//       type: "JOIN_GAME",
//       game_id: gameId
//     });
//   }
//   console.log("userId:", "currentGameId:", currentGameId.value);
// }

function playCard(cardIndex: number) {
  if (myPlayerId.value === null) return;
  if (!isMyTurn.value) {
    cannotPlayMessage.value = 'Ce n\'est pas ton tour ! ⏳';
    setTimeout(() => cannotPlayMessage.value = '', 3000);
    return;
  }
  
  // Check if card is playable (only for non-wild cards)
  if (!playableIndices.value.has(cardIndex)) {
    const card = myHand.value[cardIndex];
    cannotPlayMessage.value = `La carte ${card.colour} ${card.value} n\'est pas jouable avec la couleur actuelle !`;
    setTimeout(() => cannotPlayMessage.value = '', 3000);
    return;
  }
  
  getSocket()?.emit("message", {
    type: "PLAY_CARD",
    game_id: currentGameId.value,
    player_id: myPlayerId.value,
    card_index: cardIndex
  });
}

function drawCard() {
  if (myPlayerId.value === null) return;
  getSocket()?.emit("message", {
    type: "DRAW_CARD",
    game_id: currentGameId.value,
    player_id: myPlayerId.value
  });
}

function chooseColour(color: string) {
  if (myPlayerId.value === null) return;
  getSocket()?.emit("message", {
    type: "CHOOSE_COLOUR",
    game_id: currentGameId.value,
    player_id: myPlayerId.value,
    colour: color
  });
  showColorPicker.value = false;
}

const getCardStyle = (colour: string) => {
  if (!colour) return { backgroundColor: '#333', color: '#fff' };
  const lower = colour.toLowerCase();
  let bg = '#333';
  if (lower === 'red') bg = '#dc3545';
  if (lower === 'blue') bg = '#0d6efd';
  if (lower === 'green') bg = '#198754';
  if (lower === 'yellow') bg = '#ffc107';
  return { backgroundColor: bg, color: '#fff', border: '2px solid #fff' };
};

function isPlayerWinner(playerNumber: number): boolean {
  return gameWinner.value === playerNumber;
}

function getDirectionArrowIcon(): string {
  // direction: 1 = clockwise, -1 = counter-clockwise
  if (!gameState.value) return '→';
  return gameState.value.direction === 1 ? '→' : '←';
}

function closeGameOverModal() {
  router.push('/');
  showGameOverModal.value = false;
}
</script>

<template>
  <div class="play-container container-fluid bg-dark text-white d-flex flex-column p-2 overflow-hidden" style="height: calc(100vh - 80px);">
    <!-- INFO BAR -->
    <div class="row mb-2">
      <div class="col d-flex justify-content-between align-items-center bg-secondary p-2 rounded">
        <div><h4 class="m-0">{{ $t('play.game_title', { id: currentGameId }) }}</h4></div>
        <div v-if="gameState" class="text-center">
          <div class="d-flex align-items-center gap-2 justify-content-center">
            <span class="badge" :style="{ backgroundColor: isMyTurn ? '#28a745' : '#dc3545', fontSize: '0.9rem' }">
              {{ isMyTurn ? $t('play.your_turn') : $t('play.waiting') }}
            </span>
            <span class="fs-4 fw-bold" v-if="gameState.direction !== undefined">{{ getDirectionArrowIcon() }}</span>
          </div>
          <small class="text-muted" v-if="!isMyTurn">{{ $t('play.player_turn', { turn: gameState.current_turn }) }}</small>
        </div>
        <div>
          <button @click="router.push('/')" class="btn btn-outline-light btn-sm">{{ $t('play.quit') }}</button>
        </div>
      </div>
    </div>

    <!-- ERROR MESSAGE -->
    <div v-if="cannotPlayMessage" class="alert alert-danger alert-dismissible fade show mx-2 mb-2" role="alert">
      <strong>⚠️ {{ cannotPlayMessage }}</strong>
      <button type="button" class="btn-close" @click="cannotPlayMessage = ''" :aria-label="$t('common.close_menu')"></button>
    </div>

    <!-- OPPONENT TOP HAND -->
    <div class="row justify-content-center mb-1 align-items-center">
      <div class="col-auto text-center" v-if="oppTop">
          <div class="d-flex justify-content-center align-items-center gap-3 mb-2">
          <img v-if="playersAvatars[oppTop.player_id]" :src="playersAvatars[oppTop.player_id]" class="rounded-circle border border-2 border-secondary shadow" style="width: 50px; height: 50px; object-fit: cover;" alt="Opponent Avatar" />
          <h5 v-else class="text-secondary mb-0">{{ getPlayerName(oppTop) }}</h5>
        </div>
        <div class="d-flex gap-1 justify-content-center">
          <div v-for="n in oppTop.hand_size" :key="'oppTop_'+n" style="width: 40px;">
            <CardView :card="null" :faceDown="true" />
          </div>
        </div>
      </div>
      <div class="col-auto text-center" v-else>
        <div class="text-muted fst-italic mt-4">{{ $t('play.waiting_opponents') }}</div>
      </div>
    </div>

    <!-- CENTER PLAY AREA & SIDE PLAYERS -->
    <div class="row justify-content-between align-items-center flex-grow-1 w-100 m-0">
      
      <!-- LEFT PLAYER (Opponent 3) -->
      <div class="col-2 text-center d-flex flex-column align-items-center px-0">
        <template v-if="oppLeft">
          <img v-if="playersAvatars[oppLeft.player_id]" :src="playersAvatars[oppLeft.player_id]" class="rounded-circle border border-2 border-secondary shadow mb-2" style="width: 50px; height: 50px; object-fit: cover;" alt="Avatar" />
          <h6 v-else class="text-secondary mb-2">{{ getPlayerName(oppLeft) }}</h6>
          <!-- Stacked cards -->
          <div class="d-flex flex-column align-items-center position-relative mb-2" style="height: 100px; width: 60px;">
            <div v-for="n in oppLeft.hand_size" :key="'oppLeft_'+n" class="position-absolute" :style="{ top: (n * 12) + 'px' }">
              <CardView :card="null" :faceDown="true" style="width: 45px; transform: rotate(90deg);" />
            </div>
          </div>
        </template>
      </div>

      <!-- CENTER DECK -->
      <div class="col-6 d-flex justify-content-center gap-4 align-items-center flex-column">
        <!-- Direction Arrow -->
        <div v-if="gameState" class="d-flex gap-3 align-items-center justify-content-center">
          <img v-if="gameState.direction === 1" src="/right.png" alt="Direction clockwise" style="width: 40px; height: 40px;" />
          <img v-else-if="gameState.direction === -1" src="/left.png" alt="Direction counter-clockwise" style="width: 40px; height: 40px;" />
        </div>
        
        <!-- Cards and Deck -->
        <div class="d-flex justify-content-center gap-4 align-items-center">
          <!-- PIOCHE -->
          <button @click="drawCard" class="btn p-0 border-0" style="outline: none;">
            <CardView :card="null" :faceDown="true" hoverable style="width: 80px;" />
          </button>

          <!-- DEFAUSSE -->
          <div v-if="gameState?.top_discard" class="shadow-lg" style="width: 80px;">
            <CardView 
              :card="gameState.top_discard" 
              :resolvedColour="gameState.current_colour" 
              style="width: 80px;" 
            />
          </div>
          <div v-else class="bg-dark rounded border border-secondary d-flex justify-content-center align-items-center shadow-lg" style="width: 80px; height: 120px;">
          </div>

          <div v-if="gameState && gameState.current_colour" class="text-center">
             <small>{{ $t('play.color') }}</small><br>
             <span class="badge" :style="getCardStyle(gameState.current_colour)">{{ gameState.current_colour }}</span>
          </div>
        </div>
      </div>

      <!-- RIGHT PLAYER (Opponent 2) -->
      <div class="col-2 text-center d-flex flex-column align-items-center px-0">
        <template v-if="oppRight">
          <img v-if="playersAvatars[oppRight.player_id]" :src="playersAvatars[oppRight.player_id]" class="rounded-circle border border-2 border-secondary shadow mb-2" style="width: 50px; height: 50px; object-fit: cover;" alt="Avatar" />
          <h6 v-else class="text-secondary mb-2">{{ getPlayerName(oppRight) }}</h6>
          <!-- Stacked cards -->
          <div class="d-flex flex-column align-items-center position-relative mb-2" style="height: 100px; width: 60px;">
            <div v-for="n in oppRight.hand_size" :key="'oppRight_'+n" class="position-absolute" :style="{ top: (n * 12) + 'px' }">
              <CardView :card="null" :faceDown="true" style="width: 45px; transform: rotate(-90deg);" />
            </div>
          </div>
        </template>
      </div>

    </div>
      <div v-if="showColorPicker" class="position-absolute top-50 start-50 translate-middle bg-dark p-4 border border-warning rounded shadow-lg z-3 text-center">
        <h5 class="mb-3 text-warning">{{ $t('play.choose_color') }}</h5>
        <div class="d-flex gap-2 justify-content-center">
            <button @click="chooseColour('Red')" class="btn btn-danger text-light">{{ $t('play.color_red') }}</button>
            <button @click="chooseColour('Blue')" class="btn text-light" style="background-color: #0d6efd;">{{ $t('play.color_blue') }}</button>
            <button @click="chooseColour('Green')" class="btn btn-success text-light">{{ $t('play.color_green') }}</button>
            <button @click="chooseColour('Yellow')" class="btn btn-warning text-dark">{{ $t('play.color_yellow') }}</button>
        </div>
      </div>
    </div>

    <!-- GAME OVER MODAL -->
    <div v-if="showGameOverModal" class="position-fixed top-0 start-0 w-100 h-100 d-flex justify-content-center align-items-center" style="background-color: rgba(0,0,0,0.8); z-index: 9999;">
      <div class="bg-dark p-5 border border-success rounded shadow-lg text-center" style="max-width: 500px;">
          <h1 v-if="isPlayerWinner(myPlayerNumber)" class="display-4 text-success mb-3">
          <img 
            src="https://media2.giphy.com/media/v1.Y2lkPTc5MGI3NjExdHQweWhtM3o3MG5vOTJuNXRqajR0NmxsaGRzeWl4b3VneTVqYWNkMyZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/xqBpxjk7CXLtm/giphy.gif" 
            alt="Alien Pixel Dance" 
            class="pixelated-img mb-2"
            style="max-width: 150px; height: auto;"
          />
          <br>
          🏆 {{ $t('play.win.title') }} 🏆
        </h1>
        <h1 v-else class="display-4 text-danger mb-3">
          <img 
            src="https://media2.giphy.com/media/v1.Y2lkPTc5MGI3NjExeDZ3ZHlkcHRqYWtmdDR3bHIyb3RwMWU2em55YnR2c21zMng1YnliNyZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/l41Ym49ppcDP6iY3C/giphy.gif"
            alt="Alien Pixel Dance" 
            class="pixelated-img mb-2"
            style="max-width: 150px; height: auto;"
          />
          <br>
          💔 {{ $t('play.defeat.title') }} 💔
        </h1>
        
        <div class="mb-4">
          <h2 class="text-warning mb-3">
            {{ $t('play.winner_text', { player: gameWinner }) }}
          </h2>
        </div>

        <div class="bg-secondary p-3 rounded mb-4">
          <p v-if="isPlayerWinner(myPlayerNumber)" class="fs-5 text-white mb-0">
            <strong>{{ $t('play.win.title') }}</strong> {{ $t('play.win.message_single') }} 🎉
          </p>
          <p v-else class="fs-5 text-white mb-0">
            <strong>{{ $t('play.defeat.title') }}</strong> {{ $t('play.defeat.message_single') }} 💪
          </p>
        </div>

        <button @click="closeGameOverModal" class="btn btn-lg btn-success fw-bold">
          {{ $t('common.return_menu') }}
        </button>
      </div>
    </div>

    <!-- MY HAND -->
    <div class="row justify-content-center pb-2">
      <div class="col-12 text-center">
        <div class="d-flex justify-content-center align-items-center gap-3 mb-1">
          <img v-if="authStore.user?.avatar" :src="authStore.user.avatar" class="rounded-circle border border-2 border-white shadow" style="width: 60px; height: 60px; object-fit: cover;" alt="My Avatar" />
          <h5 v-else class="mb-0">{{ authStore.user?.username ?? $t('play.my_hand') }}</h5>
        </div>
        <div class="d-flex gap-2 justify-content-center flex-wrap px-2">
          <div 
            v-for="{ card, originalIndex } in sortedHandWithIndices" 
            :key="originalIndex" 
            style="width: 70px;" 
            class="position-relative"
          >
             <CardView 
               :card="card" 
               :hoverable="isMyTurn && playableIndices.has(originalIndex)"
               :disabled="!isMyTurn || !playableIndices.has(originalIndex)"
               @click="playCard(originalIndex)"
               :class="{ 'playable-card': isMyTurn && playableIndices.has(originalIndex) }"
             />
          </div>
        </div>
      </div>
    </div>
</template>

<style scoped>
.play-container {
  overflow-x: hidden;
}

.card-hover:hover {
  transform: translateY(-10px) !important;
  z-index: 10;
}

.card-back {
  background-image: repeating-linear-gradient(45deg, transparent, transparent 5px, rgba(0,0,0,0.2) 5px, rgba(0,0,0,0.2) 10px);
}

.playable-card :deep(.uno-card-wrapper) {
  box-shadow: 0 0 20px 5px rgba(76, 175, 80, 0.6) !important;
  border: 3px solid #4caf50 !important;
  border-radius: 8px;
}

.playable-card :deep(.uno-card-wrapper.is-hoverable:hover) {
  box-shadow: 0 0 30px 8px rgba(76, 175, 80, 0.8) !important;
  transform: translateY(-15px) scale(1.05) !important;
}
</style>