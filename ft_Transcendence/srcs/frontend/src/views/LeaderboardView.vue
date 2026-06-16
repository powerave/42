<script setup lang="ts">
import { ref, onMounted } from 'vue';
import { useAuthStore } from '../stores/auth';

type LeaderboardApiEntry = {
  user?: {
    rank?: number;
    user_id?: string;
    wins?: number;
    losses?: number;
    total_games?: number;
  };
  userDetail?: {
    uuid?: string;
    username?: string;
    xp?: number;
    avatar?: string;
  };
};

type Leader = {
  id: string;
  rank: number;
  username: string;
  points: number;
  wins: number;
  avatar?: string;
};

const auth = useAuthStore();
const leaders = ref<Leader[]>([]);
const loading = ref<boolean>(true);
const error = ref<string | null>(null);

function normalizeLeaderboardEntry(entry: LeaderboardApiEntry, index: number): Leader {
  const user = entry.user ?? {};
  const userDetail = entry.userDetail ?? {};
  const avatar = userDetail.avatar;

  return {
    id: userDetail.uuid ?? user.user_id ?? String(index),
    rank: user.rank ?? index + 1,
    username: userDetail.username ?? 'Unknown player',
    points: userDetail.xp ?? 0,
    wins: user.wins ?? 0,
    avatar: avatar && (avatar.startsWith('http://') || avatar.startsWith('https://') || avatar.startsWith('/')) ? avatar : undefined,
  };
}

function displayRank(rank: number | undefined, index: number) {
  return rank ?? index + 1;
}

async function loadLeaderboard() {
  loading.value = true;
  error.value = null;
  try {
    const response = await auth.authFetch('/api/leaderboard/all_stats');

    if (!response.ok) {
      error.value = `Impossible de récupérer le classement (${response.status}).`;
      leaders.value = [];
      return;
    }

    const contentType = response.headers.get('content-type') || '';
    if (!contentType.includes('application/json')) {
      error.value = 'Réponse inattendue du serveur (JSON attendu).';
      leaders.value = [];
      return;
    }

    const data = await response.json();
    leaders.value = Array.isArray(data)
      ? data.map((entry: LeaderboardApiEntry, index: number) => normalizeLeaderboardEntry(entry, index))
      : [];
  } catch (e: any) {
    error.value = e?.message || String(e);
    leaders.value = [];
  } finally {
    loading.value = false;
  }
}

onMounted(() => {
  loadLeaderboard();
});
</script>

<template>
  <div class="row justify-content-center g-3">
    <div class="col-12 col-xl-8">
      <div class="card bg-secondary text-white border border-secondary">
        <div class="card-header d-flex justify-content-between align-items-center bg-success text-dark fw-bold text-uppercase">
          <span>{{ $t('leaderboard.title') }}</span>
          <div>
            <button class="btn btn-sm btn-outline-dark me-2" @click="loadLeaderboard" :disabled="loading">Refresh</button>
          </div>
        </div>
        <div class="card-body">
          <div v-if="loading" class="text-center py-4">
            <div class="spinner-border text-light" role="status"><span class="visually-hidden">{{ $t('common.loading') }}</span></div>
          </div>

          <div v-else>
            <div v-if="error" class="alert alert-warning text-dark">{{ error }}</div>

            <div v-if="leaders.length === 0" class="text-center text-muted">{{ $t('leaderboard.no_data') }}</div>

            <div v-else class="table-responsive">
              <table class="table table-dark table-striped align-middle mb-0">
                <thead>
                  <tr class="text-uppercase small text-muted">
                    <th style="width: 80px;">{{ $t('leaderboard.rank') }}</th>
                    <th>{{ $t('leaderboard.player') }}</th>
                    <!-- <th style="width: 140px;" class="text-end">{{ $t('leaderboard.points') }}</th> -->
                    <th style="width: 140px;" class="text-end">{{ $t('leaderboard.wins') }}</th>
                  </tr>
                </thead>
                <tbody>
                  <tr v-for="(p, idx) in leaders" :key="p.id">
                    <td>{{ displayRank(p.rank, idx) }}</td>
                    <td>
                      <div class="d-flex align-items-center">
                        <img v-if="p.avatar" :src="p.avatar" alt="avatar" style="width:36px;height:36px;border-radius:6px;margin-right:8px;object-fit:cover;"/>
                        <div>
                          <div class="fw-bold">{{ p.username }}</div>
                        </div>
                      </div>
                    </td>
                    <!-- <td class="text-end fw-bold">{{ p.points }}</td> -->
                    <td class="text-end fw-bold">{{ p.wins }}</td>
                  </tr>
                </tbody>
              </table>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>
