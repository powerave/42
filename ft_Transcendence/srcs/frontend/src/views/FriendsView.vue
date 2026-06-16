<script setup lang="ts">
import { ref, onMounted, onUnmounted } from 'vue'; // Added by JO
import { useAuthStore } from '../stores/auth';
import { subscribe } from '../utils/socket'; // Added by JO

type Friend = {
  id: string | number;
  username: string;
  avatar?: string;
  status?: 'online' | 'offline' | 'playing' | string;
};

const auth = useAuthStore();
const friends = ref<Friend[]>([]);
const loading = ref<boolean>(true);
const error = ref<string | null>(null);
const newEmail = ref<string>('');
const adding = ref<boolean>(false);

const unsub: Array<() => void> = []; // Added by JO

async function loadFriends() {
  loading.value = true;
  error.value = null;
  try {
    const res = await auth.authFetch('/api/friends');
    if (!res.ok) {
      error.value = "Impossible de charger la liste d'amis. Affichage d'exemple local.";
      friends.value = [
        { id: 1, username: 'Alice', status: 'online' },
        { id: 2, username: 'Bob', status: 'offline' },
        { id: 3, username: 'Carol', status: 'playing' }
      ];
      return;
    }

    const data = await res.json();
    friends.value = Array.isArray(data) ? data : [];
  } catch (e: any) {
    error.value = e?.message || String(e);
    friends.value = [];
  } finally {
    loading.value = false;
  }
}

async function addFriend() {
  if (!newEmail.value.trim()) return;
  adding.value = true;
  try {
    const res = await auth.authFetch('/api/friends/request', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ friendEmail: newEmail.value.trim() })
    });

    if (!res.ok) {
      const txt = await res.text();
      let details = txt;
      try {
        const parsed = JSON.parse(txt) as { error?: unknown };
        if (parsed && typeof parsed.error !== 'undefined') {
          details = String(parsed.error);
        }
      } catch {
        // Keep raw text when response is not JSON.
      }
      error.value = `${details}`;
      return;
    }

    const created = await res.json();
    // console.log('addFriend retour front: ', created);
    friends.value.unshift(created);
    newEmail.value = '';
  } catch (e: any) {
    error.value = e?.message || String(e);
  } finally {
    adding.value = false;
  }
}

// Added by JO
onMounted(() => {
  loadFriends();
  unsub.push(subscribe('user_status_changed', (data: { user_uuid: string, status: string }) => {
    // Find the friend in the array. Ensure you compare as strings just in case.
    const friend = friends.value.find(f => String(f.id) === String(data.user_uuid));
    
    if (friend) {
      friend.status = data.status; // Instantly updates the UI
    }
  }));
});

  onUnmounted(() => {
  // Destroy the listener when leaving the "friends" page
  unsub.forEach(u => u());
});
//added by JO online status colours
</script>

<template>
  <div class="row justify-content-center g-3">
    <div class="col-12 col-xl-8">
      <div class="card bg-secondary text-white border border-secondary">
        <div class="card-header d-flex justify-content-between align-items-center bg-success text-dark fw-bold text-uppercase">
          <span>{{ $t('friends.title') }}</span>
          <div>
            <button class="btn btn-sm btn-outline-dark me-2" @click="loadFriends" :disabled="loading">Refresh</button>
          </div>
        </div>
        <div class="card-body">
          <div class="mb-3 d-flex gap-2">
            <input v-model="newEmail" class="form-control form-control-dark" :placeholder="$t('friends.add_placeholder')" />
            <button class="btn btn-primary" @click="addFriend" :disabled="adding">{{ $t('friends.add_button') }}</button>
          </div>

          <div v-if="loading" class="text-center py-4">
            <div class="spinner-border text-light" role="status"><span class="visually-hidden">{{ $t('common.loading') }}</span></div>
          </div>

          <div v-else>
            <div v-if="error" class="alert alert-warning text-dark">{{ error }}</div>

            <div v-if="friends.length === 0" class="text-center text-muted">{{ $t('friends.no_friends') }}</div>

            <ul v-else class="list-group list-group-dark">
              <li v-for="f in friends" :key="f.id" class="list-group-item d-flex justify-content-between align-items-center">
                <div class="d-flex align-items-center">
                  <img v-if="f.avatar" :src="f.avatar" alt="avatar" style="width:40px;height:40px;border-radius:6px;margin-right:10px;object-fit:cover;"/>
                  <div>
                    <div 
                      class="small fw-bold" 
                      :class="f.status === 'online' ? 'text-success' : 'text-muted'"
                    >
                      {{ f.username }}
                    </div>
                    <div class="small text-muted">{{ f.status || 'offline' }}</div>
                  </div>
                </div>
              </li>
            </ul>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>
