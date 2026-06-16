<script setup lang="ts">
import { computed, ref, watch } from 'vue';
import { useAuthStore } from '../stores/auth';
import { useI18n } from 'vue-i18n';

const { t } = useI18n();

type AvatarRecord = {
  id: number;
  key: string;
  originalName: string;
  fullUrl: string;
  isCurrent: boolean;
};

type ApiUserProfile = {
  user_uuid?: string;
  username?: string;
  avatar?: string;
  first_name?: string;
  last_name?: string;
  email?: string;
};

type UploadResponse = {
  message?: string;
  url?: string;
  key?: string;
  fileId?: number;
  id?: number;
  error?: string;
};

type ProfileForm = {
  username: string;
  firstName: string;
  lastName: string;
  email: string;
  avatar: string;
};

type ApiHistoryPlayer = {
  result?: string;
  score?: number;
  user?: {
    uuid?: string;
    username?: string;
    avatar?: string;
  };
};

type ApiHistoryMatch = {
  game_id?: number;
  game_type?: string;
  created_at?: string;
  my_result?: string;
  players?: ApiHistoryPlayer[];
};

type MatchPlayer = {
  uuid: string;
  name: string;
  score: number;
  isMe: boolean;
  isWinner: boolean;
};

type MatchHistoryItem = {
  id: number;
  gameType: string;
  date: string;
  resultKey: 'victory' | 'defeat';
  isVictory: boolean;
  players: MatchPlayer[];
};

const authStore = useAuthStore();

const profile = ref<ProfileForm>({
  username: '',
  firstName: '',
  lastName: '',
  email: '',
  avatar: ''
});

const avatars = ref<AvatarRecord[]>([]);
const selectedAvatar = ref<AvatarRecord | null>(null);
const customAvatarInput = ref<HTMLInputElement | null>(null);
const newPassword = ref('');
const confirmPassword = ref('');
const isEditMode = ref(false);
const isLoadingProfile = ref(true);
const isLoadingAvatars = ref(true);
const isSaving = ref(false);
const isUploadingAvatar = ref(false);
const isDeletingAvatar = ref(false);
const uploadProgressPercent = ref(0);
const uploadLoadedBytes = ref(0);
const uploadTotalBytes = ref(0);
const feedback = ref('');
const feedbackType = ref<'success' | 'danger' | 'info' | ''>('');
const matchHistory = ref<MatchHistoryItem[]>([]);
const isLoadingHistory = ref(true);

const activeAvatarSource = computed(() => selectedAvatar.value?.fullUrl || profile.value.avatar || '');
const activeAvatarLabel = computed(() => selectedAvatar.value?.originalName || ' ');
const displayName = computed(() => {
const firstName = profile.value.firstName.trim();
const lastName = profile.value.lastName.trim();

  return [firstName, lastName].filter(Boolean).join(' ') || t('profile.name_placeholder');
});
const displayHandle = computed(() => profile.value.username || profile.value.email.split('@')[0] || 'joueur');
const avatarInitial = computed(() => (profile.value.firstName || profile.value.username || 'U').trim().charAt(0).toUpperCase() || 'U');
const currentUserUuid = computed(() => String(authStore.user?.user_uuid ?? authStore.user?.uuid ?? ''));

function getPlayerScoreStyle(match: MatchHistoryItem, player: MatchPlayer) {
  const isWin = player.isWinner || (player.isMe && match.isVictory);
  if (isWin) {
    return { color: '#198754' }; // class text-success
  }
  
  const losers = match.players.filter((p) => !(p.isWinner || (p.isMe && match.isVictory)));
  const uniqueScores = Array.from(new Set(losers.map((p) => Number(p.score)))).sort((a, b) => b - a);
  const rank = uniqueScores.indexOf(Number(player.score));
  
  const colors = ['#dc3545', '#b02a37', '#842029', '#58151c'];
  const index = Math.max(0, Math.min(rank, colors.length - 1));
  
  return { color: colors[index] };
}

function formatMatchDate(value?: string) {
  if (!value) {
    return '';
  }

  const date = new Date(value);
  if (Number.isNaN(date.getTime())) {
    return value;
  }

  return date.toLocaleDateString();
}

function normalizeHistoryMatch(rawMatch: ApiHistoryMatch): MatchHistoryItem {
  const myResult = String(rawMatch.my_result ?? '').toLowerCase();
  const isVictory = myResult === 'win';
  const players = Array.isArray(rawMatch.players) ? rawMatch.players : [];

  return {
    id: Number(rawMatch.game_id ?? 0),
    gameType: String(rawMatch.game_type ?? ''),
    date: formatMatchDate(rawMatch.created_at),
    resultKey: isVictory ? 'victory' : 'defeat',
    isVictory,
    players: players.map((player) => {
      const uuid = String(player.user?.uuid ?? '');
      return {
        uuid,
        name: String(player.user?.username ?? 'joueur'),
        score: Number(player.score ?? 0),
        isMe: uuid !== '' && uuid === currentUserUuid.value,
        isWinner: String(player.result ?? '').toLowerCase() === 'win'
      };
    })
  };
}

function setFeedback(message: string, type: 'success' | 'danger' | 'info') {
  feedback.value = message;
  feedbackType.value = type;
}

function clearFeedback() {
  feedback.value = '';
  feedbackType.value = '';
}

function normalizeAvatar(item: Record<string, unknown>): AvatarRecord {
  const id = Number(item.id ?? item.fileId ?? 0);
  const key = String(item.key ?? item.file_path ?? '');
  const originalName = String(item.original_name ?? item.originalName ?? item.fileName ?? 'Avatar');
  const fullUrl = String(item.full_url ?? item.fullUrl ?? item.url ?? '');
  const isCurrent = Boolean(item.is_current ?? item.isCurrent ?? false);

  return {
    id,
    key,
    originalName,
    fullUrl,
    isCurrent
  };
}

async function readResponseError(response: Response, fallbackMessage: string) {
  try {
    const payload = await response.json() as Record<string, unknown>;
    const errorPayload = payload.error;

    if (typeof errorPayload === 'string' && errorPayload.trim()) {
      return errorPayload;
    }

    if (errorPayload && typeof errorPayload === 'object') {
      const nestedMessage = (errorPayload as { message?: unknown }).message;
      if (typeof nestedMessage === 'string' && nestedMessage.trim()) {
        return nestedMessage;
      }
    }

    const message = payload.message;
    if (typeof message === 'string' && message.trim()) {
      return message;
    }
  } catch {
    return fallbackMessage;
  }

  return fallbackMessage;
}

async function loadProfile() {
  isLoadingProfile.value = true;

  try {
	const isCon = localStorage.getItem('isConnected');
	if (isCon === "null") {
      return;
	}
    const response = await authStore.authFetch('/api/profile');

    if (!response.ok) {
      throw new Error(await readResponseError(response, t('profile.error_load_profile')));
    }

    const data = (await response.json()) as ApiUserProfile;
    profile.value.username = data.username || '';
    profile.value.firstName = data.first_name || '';
    profile.value.lastName = data.last_name || '';
    profile.value.email = data.email || '';
    profile.value.avatar = data.avatar || '';
  } catch (error) {
    const message = error instanceof Error ? error.message : t('profile.error_load_profile');
    setFeedback(message, 'danger');
  } finally {
    isLoadingProfile.value = false;
  }
}

async function loadAvatars() {
  isLoadingAvatars.value = true;

  try {
    const response = await authStore.authFetch('/api/profile/avatar', {method: 'GET'});

    if (!response.ok) {
      throw new Error(await readResponseError(response, t('profile.error_load_avatars')));
    }

    const payload = await response.json();
    const rawList = Array.isArray(payload)
      ? payload
      : Array.isArray(payload?.avatars)
        ? payload.avatars
        : Array.isArray(payload?.data)
          ? payload.data
          : [];

    const normalized = rawList
      .map((item: Record<string, unknown>) => normalizeAvatar(item))
      .filter((avatar: AvatarRecord) => avatar.key || avatar.fullUrl);

    avatars.value = normalized;
    selectedAvatar.value = normalized.find((avatar) => avatar.isCurrent) || null;
  } catch (error) {
    const message = error instanceof Error ? error.message : t('profile.error_load_avatars');
    setFeedback(message, 'danger');
  } finally {
    isLoadingAvatars.value = false;
  }
}

async function loadMatchHistory() {
  isLoadingHistory.value = true;

  try {
    const response = await authStore.authFetch('/api/profile/my_history');

    if (!response.ok) {
      throw new Error(await readResponseError(response, t('profile.error_load_history')));
    }

    const payload = await response.json();
    const rawList = Array.isArray(payload)
      ? payload
      : Array.isArray(payload?.history)
        ? payload.history
        : Array.isArray(payload?.data)
          ? payload.data
          : [];

    matchHistory.value = rawList
      .map((item: ApiHistoryMatch) => normalizeHistoryMatch(item))
      .filter((item: MatchHistoryItem) => item.id > 0);
  } catch (error) {
    matchHistory.value = [];
    const message = error instanceof Error ? error.message : t('profile.error_load_history');
    setFeedback(message, 'danger');
  } finally {
    isLoadingHistory.value = false;
  }
}

async function refreshProfileData() {
  await Promise.allSettled([loadProfile(), loadAvatars(), loadMatchHistory()]);
}

function resetProfileState() {
  profile.value = {
    username: '',
    firstName: '',
    lastName: '',
    email: '',
    avatar: ''
  };
  avatars.value = [];
  matchHistory.value = [];
  selectedAvatar.value = null;
  newPassword.value = '';
  confirmPassword.value = '';
  isEditMode.value = false;
  clearFeedback();
}

watch(
  () => [authStore.isAuthenticated, authStore.user, authStore.accessToken],
  ([isAuthenticated, user, accessToken]) => {
    if (!isAuthenticated && !user && !accessToken) {
      resetProfileState();
      isLoadingProfile.value = false;
      isLoadingAvatars.value = false;
      isLoadingHistory.value = false;
      return;
    }

    void refreshProfileData();
  },
  { immediate: true }
);

function selectAvatar(avatar: AvatarRecord) {
  selectedAvatar.value = avatar;
  clearFeedback();
}

function openCustomAvatarPicker() {
  customAvatarInput.value?.click();
}

function formatBytes(bytes: number) {
  if (!Number.isFinite(bytes) || bytes <= 0) {
    return '0 o';
  }

  const units = ['o', 'Ko', 'Mo', 'Go'];
  const exponent = Math.min(Math.floor(Math.log(bytes) / Math.log(1024)), units.length - 1);
  const value = bytes / (1024 ** exponent);
  const precision = value >= 10 || exponent === 0 ? 0 : 1;

  return `${value.toFixed(precision)} ${units[exponent]}`;
}

const uploadProgressText = computed(() => {
  if (!uploadTotalBytes.value) {
    return '0%';
  }

  return `${uploadProgressPercent.value}% (${formatBytes(uploadLoadedBytes.value)} / ${formatBytes(uploadTotalBytes.value)})`;
});

function resetUploadProgress() {
  isUploadingAvatar.value = false;
  uploadProgressPercent.value = 0;
  uploadLoadedBytes.value = 0;
  uploadTotalBytes.value = 0;
}

function uploadAvatarWithProgress(file: File): Promise<AvatarRecord> {
  return new Promise((resolve, reject) => {
    const xhr = new XMLHttpRequest();
    const formData = new FormData();

    formData.append('file', file);
    // formData.append('category', 'avatar');

    isUploadingAvatar.value = true;
    uploadProgressPercent.value = 0;
    uploadLoadedBytes.value = 0;
    uploadTotalBytes.value = file.size;

    xhr.open('POST', '/api/profile/avatar');
    xhr.withCredentials = true;

    xhr.upload.onprogress = (event) => {
      if (!event.lengthComputable || !event.total) {
        return;
      }

      uploadLoadedBytes.value = event.loaded;
      uploadTotalBytes.value = event.total;
      uploadProgressPercent.value = Math.min(100, Math.round((event.loaded / event.total) * 100));
    };

    xhr.onload = () => {
      isUploadingAvatar.value = false;

      let payload: UploadResponse | null = null;

      try {
        payload = xhr.responseText ? JSON.parse(xhr.responseText) as UploadResponse : null;
      } catch {
        payload = null;
      }

      if (xhr.status < 200 || xhr.status >= 300) {
        const serverError = payload?.error || t('profile.error_upload_avatar', { status: xhr.status });
        reject(new Error(serverError));
        return;
      }

      uploadProgressPercent.value = 100;
      uploadLoadedBytes.value = uploadTotalBytes.value;

      resolve({
        id: Number(payload?.fileId ?? payload?.id ?? 0),
        key: String(payload?.key ?? ''),
        originalName: file.name,
        fullUrl: String(payload?.url ?? ''),
        isCurrent: false
      });

      // Attendre 2 secondes pour afficher la fin de la progression
      setTimeout(() => {
        resetUploadProgress();
      }, 2000);
    };

    xhr.onerror = () => {
      isUploadingAvatar.value = false;
      reject(new Error(t('profile.error_upload_network')));
    };

    xhr.onabort = () => {
      isUploadingAvatar.value = false;
      reject(new Error(t('profile.error_upload_aborted')));
    };

    xhr.send(formData);
  });
}

async function onCustomAvatarSelected(event: Event) {
  const input = event.target as HTMLInputElement | null;
  const file = input?.files?.[0];

  if (!file) {
    return;
  }

  input.value = '';
  clearFeedback();

  const MAX_FILE_SIZE = 950; // en Ko

  if (file.size > MAX_FILE_SIZE * 1024) {
    setFeedback(t('profile.error_file_too_large'), 'danger');
    return;
  }

  try {
    const uploadedAvatar = await uploadAvatarWithProgress(file);

    avatars.value = [uploadedAvatar, ...avatars.value.filter((avatar) => avatar.key !== uploadedAvatar.key)];
    selectedAvatar.value = uploadedAvatar;
    profile.value.avatar = uploadedAvatar.fullUrl || profile.value.avatar;
    setFeedback(t('profile.upload_success'), 'success');
  } catch (error) {
    const errorMessage = error instanceof Error ? error.message : '';

    if (errorMessage.includes('HTTP 401')) {
      if (!authStore.isAuthenticated) {
        try {
          const uploadedAvatar = await uploadAvatarWithProgress(file);
          avatars.value = [uploadedAvatar, ...avatars.value.filter((avatar) => avatar.key !== uploadedAvatar.key)];
          selectedAvatar.value = uploadedAvatar;
          profile.value.avatar = uploadedAvatar.fullUrl || profile.value.avatar;
          setFeedback(t('profile.upload_success'), 'success');
          return;
        } catch (retryError) {
          const retryMessage = retryError instanceof Error ? retryError.message : t('profile.error_upload_failed');
          setFeedback(retryMessage, 'danger');
          return;
        }
      }
    }

    const message = error instanceof Error ? error.message : t('profile.error_upload_failed');
    setFeedback(message, 'danger');
  }
}

async function deleteAvatar(avatar: AvatarRecord) {
  if (!avatar.id || isDeletingAvatar.value) {
    return;
  }

if (typeof window !== 'undefined' && !window.confirm(t('profile.confirm_delete_avatar', { name: avatar.originalName }))) {
      return;
  }

  isDeletingAvatar.value = true;
  clearFeedback();

  try {
    const response = await authStore.authFetch(`/api/files/${avatar.id}`, {
      method: 'DELETE'
    });

    if (!response.ok) {
      throw new Error(await readResponseError(response, t('profile.error_delete_avatar')));
    }

    if (selectedAvatar.value?.id === avatar.id) {
      selectedAvatar.value = null;
    }

    await refreshProfileData();
    setFeedback(t('profile.delete_success'), 'success');
  } catch (error) {
    const message = error instanceof Error ? error.message : t('profile.error_delete_avatar');
    setFeedback(message, 'danger');
  } finally {
    isDeletingAvatar.value = false;
  }profile
}

function cancelEditing() {
  isEditMode.value = false;
  void refreshProfileData();
}

async function confirmProfile() {
  const username = profile.value.username.trim();
  const firstName = profile.value.firstName.trim();
  const lastName = profile.value.lastName.trim();
  const email = profile.value.email.trim();

  if (!username || !firstName || !lastName || !email) {
    setFeedback(t('profile.error_required_fields'), 'danger');
    return;
  }

  if (newPassword.value && newPassword.value !== confirmPassword.value) {
    setFeedback(t('profile.error_password_mismatch'), 'danger');
    return;
  }

  isSaving.value = true;
  clearFeedback();

  try {
    const payload: Record<string, string> = {
      username,
      first_name: firstName,
      last_name: lastName,
      email
    };

    if (selectedAvatar.value?.key) {
      payload.avatar = selectedAvatar.value.key;
    }

    if (newPassword.value.trim()) {
      payload.password = newPassword.value.trim();
    }

    const response = await authStore.authFetch('/api/profile', {
      method: 'PATCH',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(payload)
    });

    if (!response.ok) {
      throw new Error(await readResponseError(response, t('profile.error_save_failed')));
    }

    newPassword.value = '';
    confirmPassword.value = '';
    isEditMode.value = false;

    await refreshProfileData();
    setFeedback(t('profile.save_success'), 'success');
  } catch (error) {
    const message = error instanceof Error ? error.message : t('profile.error_save_failed');
    setFeedback(message, 'danger');
  } finally {
    isSaving.value = false;
  }
}
</script>

<template>
  <div class="container-fluid px-0">
    <div class="row g-4 m-0 justify-content-center">
      <div class="col-12" :class="(!isEditMode && !isLoadingProfile) ? 'col-lg-5 col-xl-4' : ''">
        <div class="profile-shell w-100">
          <div v-if="isLoadingProfile && !profile.email" class="card profile-card bg-dark text-white border border-info overflow-hidden mx-auto" style="max-width: 400px; border-radius: 12px;">
      <div class="card-body p-4 p-lg-5 d-flex align-items-center justify-content-center text-center">
        <div>
          <div class="spinner-border text-info mb-3" role="status" aria-hidden="true"></div>
          <div class="fw-semibold">{{ $t('profile.loading') }}</div>
        </div>
      </div>
    </div>

    <div v-else-if="!isEditMode" class="card profile-card bg-dark text-white border border-info overflow-hidden mx-auto" style="max-width: 400px; border-radius: 12px;">
      <div class="card-body p-4 p-lg-5 d-flex flex-column align-items-center text-center">
        <div class="profile-avatar-preview d-flex align-items-center justify-content-center overflow-hidden rounded-circle border border-info mb-3" style="width: 120px; height: 120px; border-width: 3px !important; box-shadow: 0 0 15px rgba(0, 204, 255, 0.2);">
          <img v-if="activeAvatarSource" :src="activeAvatarSource" :alt="activeAvatarLabel" class="w-100 h-100 object-fit-cover">
          <span v-else class="fw-bold fs-1 text-info">{{ avatarInitial }}</span>
        </div>

        <h2 class="h4 fw-bold mb-0">
          {{ displayName }}
        </h2>
        <div class="text-secondary small fw-semibold mb-4">
          @{{ displayHandle }}
        </div>

        <button class="btn btn-outline-info w-100 py-2 fw-bold text-uppercase mb-4" type="button" @click="isEditMode = true">
          {{ $t('profile.edit_profile') }}
        </button>

        <div class="w-100 border-top border-secondary pt-4 text-start">
          <div class="d-flex justify-content-between align-items-center py-2 border-bottom border-dark">
            <div class="d-flex align-items-center gap-2 text-secondary">
              <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-geo-alt-fill" viewBox="0 0 16 16">
                <path d="M8 16s6-5.686 6-10A6 6 0 0 0 2 6c0 4.314 6 10 6 10zm0-7a3 3 0 1 1 0-6 3 3 0 0 1 0 6z"/>
              </svg>
              <span class="small fw-semibold">{{ $t('profile.email') }}</span>
            </div>
            <strong class="small">{{ profile.email }}</strong>
          </div>
          <div class="d-flex justify-content-between align-items-center py-2 border-bottom border-dark">
            <div class="d-flex align-items-center gap-2 text-secondary">
              <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-person-fill" viewBox="0 0 16 16">
                <path d="M3 14s-1 0-1-1 1-4 6-4 6 3 6 4-1 1-1 1H3zm5-6a3 3 0 1 0 0-6 3 3 0 0 0 0 6z"/>
              </svg>
              <span class="small fw-semibold">{{ $t('profile.username') }}</span>
            </div>
            <strong class="small">{{ displayHandle }}</strong>
          </div>
        </div>
      </div>
    </div>

    <div v-else class="card profile-card bg-dark text-white border border-info overflow-hidden mx-auto">
      <div class="card-header profile-card-header bg-info text-light fw-bold text-uppercase py-3 px-4 px-lg-5 d-flex flex-wrap align-items-center justify-content-between gap-2">
        <span>{{ $t('profile.edit_profile') }}</span>
        <button class="btn btn-sm btn-light fw-bold" type="button" @click="cancelEditing">{{ $t('profile.cancel') }}</button>
      </div>

      <div class="card-body profile-card-body p-4 p-lg-5">
        <form class="d-flex flex-column gap-4" @submit.prevent="confirmProfile">
          <div v-if="feedback" class="alert mb-0" :class="feedbackType === 'success' ? 'alert-success' : feedbackType === 'info' ? 'alert-info' : 'alert-danger'" role="status">
            {{ feedback }}
          </div>

          <div class="row g-4 align-items-start">
            <div class="col-12 col-lg-4">
              <div class="profile-avatar-panel h-100 rounded-4 border border-secondary p-4 d-flex flex-column gap-4">
                <div class="d-flex flex-column align-items-center text-center gap-3">
                  <div class="profile-avatar-preview d-flex align-items-center justify-content-center overflow-hidden">
                    <img v-if="activeAvatarSource" :src="activeAvatarSource" :alt="activeAvatarLabel" class="w-100 h-100 object-fit-cover">
                    <span v-else class="fw-bold fs-2">{{ avatarInitial }}</span>
                  </div>
                  <div>
                    <div class="text-secondary text-uppercase small fw-semibold">{{ $t('profile.photo') }}</div>
                    <div class="h5 fw-bold mb-1">{{ activeAvatarLabel }}</div>
                    <p class="text-secondary mb-0 small">{{ $t('profile.choose_avatar') }}</p>
                  </div>
                </div>

                <div class="d-flex flex-column gap-3">
                  <div class="d-flex align-items-center justify-content-between gap-2">
                    <div class="text-secondary small text-uppercase fw-semibold">{{ $t('profile.available_avatars') }}</div>
                    <div class="text-secondary small">{{ avatars.length }} photo(s)</div>
                  </div>

                  <div v-if="isLoadingAvatars" class="text-secondary small">{{ $t('profile.loading_avatars') }}</div>
                  <div v-else class="profile-avatar-strip d-flex gap-2 overflow-auto pb-1">
                    <template v-if="avatars.length">
                      <div v-for="avatar in avatars" :key="avatar.id" class="position-relative flex-shrink-0">
                        <button
                          class="profile-avatar-choice btn p-2 flex-shrink-0"
                          :class="selectedAvatar?.id === avatar.id ? 'is-selected' : ''"
                          type="button"
                          @click="selectAvatar(avatar)"
                        >
                          <img :src="avatar.fullUrl" :alt="avatar.originalName" class="profile-avatar-choice-image">
                          <span class="d-block mt-2 small fw-semibold text-truncate">{{ avatar.originalName }}</span>
                        </button>
                        <div v-if="avatar.isCurrent" class="badge bg-info text-dark position-absolute bottom-0 start-50 translate-middle-x">{{ $t('profile.current_avatar') }}</div>
                      </div>
                    </template>
                    <div v-else class="text-secondary small">{{ $t('profile.no_avatars') }}</div>
                  </div>
                </div>

                <div class="d-flex flex-column gap-2 mt-2">
				  <p class="text-secondary mb-0 small">{{ $t('profile.size_max') }}</p>
                  <button class="btn btn-outline-info fw-bold text-uppercase" type="button" @click="openCustomAvatarPicker">
                    {{ $t('profile.add_photo') }}
                  </button>
                  <input ref="customAvatarInput" class="visually-hidden" type="file" accept="image/*" @change="onCustomAvatarSelected">

                  <div v-if="isUploadingAvatar || uploadTotalBytes > 0" class="d-flex flex-column gap-2 mt-1">
                    <div class="d-flex align-items-center justify-content-between small">
                      <span class="text-secondary">{{ $t('profile.upload_progress') }}</span>
                      <span class="text-info fw-bold">{{ uploadProgressText }}</span>
                    </div>
                    <div class="progress profile-upload-progress" role="progressbar" aria-label="Progression upload avatar" :aria-valuenow="uploadProgressPercent" aria-valuemin="0" aria-valuemax="100">
                      <div class="progress-bar bg-info" :style="{ width: `${uploadProgressPercent}%` }">{{ uploadProgressPercent }}%</div>
                    </div>
                  </div>
                </div>
              </div>
            </div>

            <div class="col-12 col-lg-8">
              <div class="d-flex flex-column gap-3">
                <div class="row g-3">
                  <div class="col-12 col-md-6">
                    <label for="username" class="form-label fw-semibold">{{ $t('profile.username') }}</label>
                    <input id="username" v-model="profile.username" type="text" class="form-control form-control-lg profile-input" autocomplete="username" placeholder="player42" required>
                  </div>
                  <div class="col-12 col-md-6">
                    <label for="first-name" class="form-label fw-semibold">{{ $t('profile.firstname') }}</label>
                    <input id="first-name" v-model="profile.firstName" type="text" class="form-control form-control-lg profile-input" autocomplete="given-name" placeholder="Clara" required>
                  </div>
                  <div class="col-12 col-md-6">
                    <label for="last-name" class="form-label fw-semibold">{{ $t('profile.lastname') }}</label>
                    <input id="last-name" v-model="profile.lastName" type="text" class="form-control form-control-lg profile-input" autocomplete="family-name" placeholder="Dev" required>
                  </div>
                  <div class="col-12 col-md-6">
                    <label for="email" class="form-label fw-semibold">{{ $t('profile.email') }}</label>
                    <input id="email" v-model="profile.email" type="email" class="form-control form-control-lg profile-input" autocomplete="email" placeholder="clara@uno.dev" required>
                  </div>

                  <div class="col-12 mt-3">
                    <h5 class="h6 fw-bold border-bottom border-secondary pb-2 mb-3">{{ $t('profile.change_password') }}</h5>
                  </div>

                  <div class="col-12 col-md-6">
                    <label for="new-password" class="form-label fw-semibold">{{ $t('profile.new_password') }}</label>
                    <input id="new-password" v-model="newPassword" type="password" class="form-control form-control-lg profile-input" autocomplete="new-password" placeholder="••••••••">
                  </div>
                  <div class="col-12 col-md-6">
                    <label for="confirm-password" class="form-label fw-semibold">{{ $t('profile.confirm_password') }}</label>
                    <input id="confirm-password" v-model="confirmPassword" type="password" class="form-control form-control-lg profile-input" autocomplete="new-password" placeholder="••••••••">
                  </div>
                </div>

                <div class="d-flex justify-content-center justify-content-lg-end pt-4 gap-3">
                  <button class="btn btn-light btn-lg fw-bold text-uppercase px-4" type="button" @click="cancelEditing">
                    {{ $t('profile.cancel') }}
                  </button>
                  <button class="btn btn-primary btn-lg fw-bold text-uppercase px-4" type="submit" :disabled="isSaving || isUploadingAvatar || isDeletingAvatar">
                    {{ isSaving ? $t('profile.saving') : $t('profile.confirm') }}
                  </button>
                </div>
              </div>
            </div>
          </div>
        </form>
      </div>
    </div>

        </div> <!-- Close profile-shell -->
      </div> <!-- Close col-12 profile wrapper -->

      <!-- Match History Section -->
      <div v-if="!isEditMode && !isLoadingProfile" class="col-12 col-lg-7 col-xl-6">
        <div class="card bg-dark text-white border border-info h-100 mx-auto" style="border-radius: 12px; max-width: 800px;">
          <div class="card-header bg-info text-dark fw-bold text-uppercase py-3 px-4 d-flex align-items-center justify-content-between gap-2">
            <span>{{ $t('profile.match_history') }}</span>
            <span class="badge bg-dark text-info">{{ $t('profile.recent') }}</span>
          </div>
          <div class="card-body p-4 overflow-auto" style="max-height: 600px;">
            <div v-if="isLoadingHistory" class="text-secondary text-center">
              {{ $t('common.loading') }}
            </div>

            <div v-else-if="matchHistory.length === 0" class="text-secondary text-center">
              {{ $t('profile.no_matches') }}
            </div>
            
            <div v-for="match in matchHistory" :key="match.id" class="card border border-secondary mb-3 bg-transparent text-white">
              <div class="card-body p-3 d-flex flex-column flex-sm-row justify-content-between align-items-center gap-3">
                <div class="d-flex flex-column text-center text-sm-start" style="min-width: 100px;">
                  <span class="fs-5 fw-bold" :class="match.isVictory ? 'text-success' : 'text-danger'">
                    {{ $t(`profile.${match.resultKey}`) }}
                  </span>
                  <span class="small text-secondary">{{ match.date }}</span>
                </div>
                
                <div class="d-flex flex-wrap align-items-center justify-content-center gap-2 flex-grow-1">
                  <template v-for="(player, index) in match.players" :key="index">
                    <div class="text-center" style="min-width: 80px; overflow: hidden;">
                      <span class="d-block fw-semibold text-truncate mx-auto" style="max-width: 100px;" :class="player.isMe ? 'text-info' : 'text-light'">
                        {{ player.isMe ? displayHandle : player.name }}
                      </span>
                      <span class="fs-4 d-block fw-bold" :style="getPlayerScoreStyle(match, player)">
                        {{ player.score }}
                      </span>
                    </div>
                    
                    <span v-if="Number(index) < match.players.length - 1" class="text-white-50 fw-bold fs-6 mx-1">VS</span>
                  </template>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>

    </div>
  </div>
</template>
