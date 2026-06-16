<script setup lang="ts">
const emit = defineEmits<{
  (e: 'toggle-sidebar'): void;
}>();

import { computed } from 'vue';
import { useAuthStore } from '../stores/auth';

const authStore = useAuthStore();

const currentXp = computed(() => {
  return authStore.user?.xp || 0; 
});

// EXP calculation
const levelStats = computed(() => {
  let xp = currentXp.value;
  let lvl = 1;
  let nextLevelReq = 10;

  while (xp >= nextLevelReq && lvl < 100) {
    lvl++;
    xp -= nextLevelReq;
    nextLevelReq = 10 + (2 * (lvl - 1));
  }

  if (lvl >= 100) {
    xp = 0;
    nextLevelReq = 0; 
  }

  return { lvl, currentLevelXp: xp, nextLevelReq };
});

const currentLevel = computed(() => levelStats.value.lvl);

const currentLevelXp = computed(() => levelStats.value.currentLevelXp);

const nextLevelXp = computed(() => levelStats.value.nextLevelReq);

const xpPercent = computed(() => {
  if (currentLevel.value >= 100) return 100; 
  if (nextLevelXp.value === 0) return 0;
  return Math.min(100, (currentLevelXp.value / nextLevelXp.value) * 100);
});

const avatarInitial = computed(() => {
  const name = authStore.user?.username || authStore.user?.first_name || 'U';
  return name.trim().charAt(0).toUpperCase();
});

function handleToggleSidebar() {
  emit('toggle-sidebar');
}

async function handleLogout() {
  await authStore.logout();
  window.location.hash = '#/login';
}
</script>

<template>
  <header class="welcome-banner navbar navbar-expand-lg navbar-dark py-3 justify-content-end position-relative">
    <div class="header-inner d-flex align-items-start w-100">
      <button
        class="mobile-menu-btn btn btn-dark d-md-none"
        type="button"
        aria-label="Ouvrir le menu"
        @click="handleToggleSidebar"
      >
        ☰
      </button>

      <div class="header-actions d-flex align-items-center gap-3">
      <!-- Language Switcher -->
      <select v-model="$i18n.locale" class="form-select form-select-sm bg-dark text-white border-secondary" style="width: auto;">
        <option value="fr">{{ $t('nav.lang_fr') }}</option>
        <option value="en">{{ $t('nav.lang_en') }}</option>
        <option value="de">{{ $t('nav.lang_de') }}</option>
      </select>

      <div class="d-flex align-items-center gap-2">
        <div v-if="!authStore.isAuthenticated" class="d-flex flex-column gap-2">
          <a class="btn btn-dark btn-sm fw-bold text-uppercase" href="#/signup">
            {{ $t('nav.signup') }}
          </a>
          <a class="btn btn-light btn-sm fw-bold text-uppercase" href="#/login">
            {{ $t('nav.login') }}
          </a>
        </div>
		<div v-else class="d-flex align-items-center gap-2">
      
      <a href="#/profile" class="d-inline-block rounded-circle overflow-hidden border border-2 border-secondary ms-2 me-2" style="width: 45px; height: 45px; text-decoration: none;">
        <img v-if="authStore.user?.avatar" :src="authStore.user.avatar" alt="Avatar" class="w-100 h-100 object-fit-cover" />
        <div v-else class="w-100 h-100 bg-secondary d-flex align-items-center justify-content-center text-white fw-bold fs-5">
          {{ avatarInitial }}
        </div>
      </a>

		  <div class="d-flex flex-column gap-2">
        <button class="btn btn-dark btn-sm fw-bold text-uppercase" type="button" @click="handleLogout">
        {{ $t('nav.logout') }}
        </button>
		  </div>
		</div>
      </div>
    </div>
    </div>

    <!-- simple EXP bar -->
    <div v-if="authStore.isAuthenticated" class="xp-wrapper d-flex align-items-center justify-content-end w-100 position-absolute" style="bottom: 30px;">
      <!-- LvL-->
      <span class="text-white fw-bold me-2 mb-0 fst-italic shadow-text" style="font-size: 1.1rem; line-height: 1;">
        LvL.{{ currentLevel }}
      </span>

      <!-- bar -->
      <div class="xp-bar-container m-0">
        <div class="xp-bar" :style="{ width: `${xpPercent}%` }"></div>
        <!-- Texte de progression à l'intérieur -->
        <span class="xp-text position-absolute w-100 text-center text-white fw-bold" style="top: 0; left: 0; font-size: 0.8rem; line-height: 25px; text-shadow: 1px 1px 2px black;">
          <template v-if="currentLevel >= 100">
            Niveau Max Atteint !
          </template>
          <template v-else>
            {{ currentLevelXp }} / {{ nextLevelXp }} XP
          </template>
        </span>
      </div>
    </div>
  </header>
</template>

<style scoped>
.welcome-banner {
  background: var(--bs-heading);
  height: 220px;
  background-image: url('../../assets/banniere.png');
  background-size: cover;
  background-position: center;
  padding-left: 0;
  padding-right: 0;
  border: none !important;
}

.header-inner {
  min-width: 0;
}

.header-actions, .xp-wrapper {
  margin-left: auto;
  margin-right: 0;
  padding-right: 5%;
}

.mobile-menu-btn {
  margin-left: 0.75rem;
  font-size: 1.4rem;
  line-height: 1;
  padding: 0.4rem 0.7rem;
}

@media (max-width: 767.98px) {
  .welcome-banner {
    height: 160px;
  }

  .header-actions, .xp-wrapper {
    padding-right: 5%;
  }
}

.no-underline {
  text-decoration: none !important;
}

.xp-bar-container {
  position: relative;
  width: 15%; 
  min-width: 150px;
  height: 25px;
  background-color: rgba(6, 7, 8, 0.7); 
  border: 1px solid rgba(107, 102, 102, 0.1);
  overflow: hidden;
  border-radius: 4px;
}

.xp-bar {
  height: 100%;
  background: linear-gradient(45deg, #251be0, #33da0a, #ebd10d, #d70d0d);
  transition: width 0.5s ease-out;
  box-shadow: 0 0 8px rgba(255, 255, 255, 0.8);
}

.shadow-text {
  text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.8);
}
</style>