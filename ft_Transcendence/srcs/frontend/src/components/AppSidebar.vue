<script setup lang="ts">
import { computed } from 'vue';
import { useRoute } from 'vue-router';

const props = defineProps<{
  mobileOpen: boolean;
}>();

const emit = defineEmits<{
  (e: 'close'): void;
}>();

const route = useRoute();

const currentName = computed(() => String(route.name || ''));

function isActive(name: string) {
  return currentName.value === name;
}

function closeSidebar() {
  emit('close');
}
</script>

<template>
  <aside class="app-sidebar bg-black text-white d-flex flex-column p-3" :class="{ 'is-mobile-open': props.mobileOpen }">
    <div class="d-flex d-md-none justify-content-end mb-2">
      <button class="btn btn-outline-light btn-sm" type="button" :aria-label="$t('common.close_menu')" @click="closeSidebar">
        ✕
      </button>
    </div>

    <div class="text-light fs-4 fw-bold text-center mb-4 text-info">ft_transcendence</div>
    <nav class="nav nav-pills flex-column gap-2">
      <RouterLink class="sidebar-item nav-link text-start text-white" :class="{ active: isActive('home'), 'bg-primary': isActive('home') }" to="/home" @click="closeSidebar"><img src="../../assets/Cards/Back.png" style="width: 35px; height: 45px; margin-right: 8px;"> {{ $t('sidebar.uno_home') }}</img></RouterLink>
      <RouterLink class="sidebar-item nav-link text-start text-white" :class="{ active: isActive('profile'), 'bg-primary': isActive('profile') }" to="/profile" @click="closeSidebar"><img src="../../assets/Cards/Green_Reverse.png" style="width: 35px; height: 45px; margin-right: 8px;"> {{ $t('sidebar.profile') }}</RouterLink>
      <RouterLink class="sidebar-item nav-link text-start text-white" :class="{ active: isActive('leaderboard'), 'bg-primary': isActive('leaderboard') }" to="/leaderboard" @click="closeSidebar"><img src="../../assets/Cards/Blue_Plus_2.png" style="width: 35px; height: 45px; margin-right: 8px;"> {{ $t('sidebar.leaderboard') }}</RouterLink>
      <RouterLink class="sidebar-item nav-link text-start text-white" :class="{ active: isActive('friends'), 'bg-primary': isActive('friends') }" to="/friends" @click="closeSidebar"><img src="../../assets/Cards/Yellow_Plus_4.png" style="width: 35px; height: 45px; margin-right: 8px;"> {{ $t('sidebar.friends') }}</RouterLink>
    </nav>
  </aside>

  <div class="sidebar-backdrop" :class="{ show: props.mobileOpen }" @click="closeSidebar" />
</template>

<style scoped>
.app-sidebar {
  width: 280px;
  min-width: 280px;
  border: none !important;
  z-index: 1045;
}

@media (max-width: 767.98px) {
  .app-sidebar {
    position: fixed;
    top: 0;
    left: 0;
    bottom: 0;
    width: min(82vw, 300px);
    min-width: min(82vw, 300px);
    transform: translateX(-100%);
    transition: transform 0.25s ease;
    overflow-y: auto;
  }

  .app-sidebar.is-mobile-open {
    transform: translateX(0);
  }

  .sidebar-backdrop {
    position: fixed;
    inset: 0;
    background: rgba(0, 0, 0, 0.5);
    opacity: 0;
    pointer-events: none;
    transition: opacity 0.25s ease;
    z-index: 1040;
  }

  .sidebar-backdrop.show {
    opacity: 1;
    pointer-events: auto;
  }
}

@media (min-width: 768px) {
  .sidebar-backdrop {
    display: none;
  }
}
</style>
