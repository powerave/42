<script setup lang="ts">
import { computed, onMounted, onUnmounted, ref, watch } from 'vue';
import { useRouter } from 'vue-router';
import { useAuthStore } from '../stores/auth';
import { fetchWithAuth } from '../utils/fetchWithAuth';
import { createGameApi, listGamesApi } from '../utils/gameApi';
import { getSocket, subscribe } from '../utils/socket';

const stats = ref({ games_count: 0, win_count: 0, ratio: 0 });

async function fetchStats() {
  if (!authStore.isAuthenticated) {
    stats.value = { games_count: 0, win_count: 0, ratio: 0 };
    return;
  }

  try {
    const response = await fetchWithAuth('/api/home/my_stats');
    if (!response.ok) {
      throw new Error(`Erreur HTTP: ${response.status}`);
    }
    const data = await response.json();
    const gamesCount = data.games_count ?? data.games ?? 0;
    const winCount = data.win_count ?? data.wins ?? 0;

    let ratioVal = typeof data.ratio === 'number' ? data.ratio : parseFloat(data.ratio ?? '');
    if (!Number.isFinite(ratioVal)) ratioVal = 0;
    stats.value.games_count = gamesCount;
    stats.value.win_count = winCount;
    stats.value.ratio = ratioVal;
  } catch (err) {
    //console.error('Erreur lors de la récupération des statistiques :', err);
  }
}
const authStore = useAuthStore();
const router = useRouter();
const isSolo = ref<boolean>(false);

// Variable to store number of max players (by default 4)
const maxPlayersSelection = ref<number>(4);
const availableGames = ref<any[]>([]);
const unsub: Array<() => void> = [];

onMounted(() => {
	if (authStore.isAuthenticated) {
  		fetchGames();
    	fetchStats();
  }
  const closeSub = subscribe('message', (data: any)=> {
      console.log("Message reçu du Home :", data.type, data);
      switch (data.type) {
        case 'NEW_GAME':
          if (data.games) {
            availableGames.value = data.games;
          } else {
            fetchGames();
          }
          break;
      }
  })
  unsub.push(closeSub);
});
 
onUnmounted(() => {
  unsub.forEach(u => u());
});
 
async function fetchGames() {
  try {
    const data = await listGamesApi();
    availableGames.value = data;
  } catch (err) {
    //console.error("Erreur lors de la récupération des parties :", err);
  }
}
 
function joinGame(gameId: number) {
  console.log(`Rejoindre la partie ${gameId}...`);
  const socket = getSocket();
  if (socket) {
    console.log(`Tentative de rejoindre la partie ${gameId}`);
    socket.emit("message", {
      type: "JOIN_GAME",
      game_id: gameId
    });
  }
  console.log("userId:", authStore.user?.id, "currentGameId:", gameId);
  router.push(`/play/${gameId}`);
}
 
async function createGame() {
  try {
    const players = maxPlayersSelection.value;
    const data = await createGameApi(players, authStore.user?.id, isSolo.value);
    console.log("Parties créée avec succès !", data);
    router.push(`/play/${data.game_id}`);
  } catch (err) {
    //console.error("Erreur réseau :", err);
  }
}
</script>
 
<template>
  <div class="row g-3">
    <div class="col-12 col-xl-4 d-flex flex-column gap-3">
      <div class="card bg-secondary text-white border border-secondary h-100">
        <div class="card-header bg-success text-dark fw-bold text-uppercase d-flex justify-content-between align-items-center">
          {{ $t('home.join_game') }}
        </div>
        <div class="card-body d-flex flex-column gap-3">
 
          <div class="p-3 bg-dark rounded border border-secondary">
 
            <!-- Toggle Solo / Multi -->
            <div class="d-flex gap-2 mb-3">
              <button
                @click="isSolo = false"
                :class="['btn', 'fw-bold', 'text-uppercase', 'flex-grow-1', isSolo ? 'btn-outline-info' : 'btn-info']"
                type="button"
              >
                🌐 Multi
              </button>
              <button
                @click="isSolo = true"
                :class="['btn', 'fw-bold', 'text-uppercase', 'flex-grow-1', isSolo ? 'btn-warning' : 'btn-outline-warning']"
                type="button"
              >
                🤖 Solo
              </button>
            </div>
            <!-- Select players numbers -->
            <label for="playerSelect" class="form-label small fw-bold text-uppercase text-secondary mb-2">{{ $t('home.num_players') }}</label>
            <select
              id="playerSelect"
              v-model.number="maxPlayersSelection"
              class="form-select bg-secondary text-white border-secondary fw-bold mb-2"
              style="width: 90px;"
            >
              <option :value="2">2</option>
              <option :value="3">3</option>
              <option :value="4">4</option>
            </select>
 
            <!-- Info bots auto (mode solo) -->
            <template if="isSolo">
              <small class="text-warning">🤖 {{ maxPlayersSelection - 1 }} bot{{ maxPlayersSelection - 1 > 1 ? 's' : '' }}</small>
            </template>
            <!-- create button -->
            <button
              @click="createGame"
              :class="['btn', 'fw-bold', 'text-uppercase', 'w-100', 'mt-3', isSolo ? 'btn-warning' : 'btn-info']"
              type="button"
            >
              {{ $t('home.create_room') }}
            </button>
 
          </div>
 
          <div class="list-group mt-2">
            <div v-for="game in availableGames" :key="game.game_id" class="list-group-item d-flex align-items-center justify-content-between bg-dark text-white border-secondary mb-2 rounded">
              <div>
                <div class="text-light fw-bold">{{ $t('home.game', { id: game.game_id }) }}</div>
                <small class="text-secondary">{{ $t('home.players_count', { current: game.current_players, max: game.max_players }) }}</small>
              </div>
              <button @click="joinGame(game.game_id)" class="btn btn-sm btn-primary fw-bold text-uppercase" type="button">{{ $t('home.join') }}</button>
            </div>
 
            <div v-if="availableGames.length === 0" class="text-center text-secondary mt-3">
              {{ $t('home.no_games') }}
            </div>
          </div>
        </div>
      </div>
    </div>
 
    <div class="col-12 col-xl-4 d-flex flex-column gap-3">
      <div class="card bg-secondary text-white border border-secondary h-100">
        <div class="card-header bg-success text-dark fw-bold text-uppercase">{{ $t('home.stats') }}</div>
        <div class="card-body d-flex flex-column gap-3">
          <div class="list-group-item d-flex align-items-center justify-content-between bg-dark text-white border-secondary rounded">
            <div>
              <div class="text-light fw-bold">{{ $t('home.nb_games_classic') }}</div>
              <small class="text-secondary">{{ $t('home.nb_games_classic_desc') }}</small>
            </div>
            <span class="text-success fs-4 fw-bold">{{ stats.games_count }}</span>
          </div>
          <div class="list-group-item d-flex align-items-center justify-content-between bg-dark text-white border-secondary rounded">
            <div>
              <div class="text-light fw-bold">{{ $t('home.victory') }}</div>
              <small class="text-secondary">{{ $t('home.victory_desc') }}</small>
            </div>
            <span class="text-success fs-4 fw-bold">{{ stats.win_count }}</span>
          </div>
          <div class="list-group-item d-flex align-items-center justify-content-between bg-dark text-white border-secondary rounded">
            <div>
              <div class="text-light fw-bold">{{ $t('home.ratio') }}</div>
              <small class="text-secondary">{{ $t('home.ratio_desc') }}</small>
            </div>
            <span class="text-primary fs-4 fw-bold">{{ stats.ratio.toFixed(2) }}%</span>
          </div>
        </div>
      </div>
    </div>
 
    <div v-if="authStore.isAuthenticated" class="col-12 col-xl-4 d-flex flex-column gap-3">
      <div class="card bg-secondary text-white border border-secondary h-100">
        <div class="card-header bg-success text-dark fw-bold text-uppercase">{{ $t('home.friends_online') }}</div>
      </div>
    </div>
  </div>
</template>
 
<style scoped>
.form-select {
  background-image: url("data:image/svg+xml,%3csvg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 16 16'%3e%3cpath fill='none' stroke='%23ffffff' stroke-linecap='round' stroke-linejoin='round' stroke-width='2' d='m2 5 6 6 6-6'/%3e%3c/svg%3e");
}
</style>