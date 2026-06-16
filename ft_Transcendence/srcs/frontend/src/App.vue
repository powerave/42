<script setup lang="ts">
import { onMounted, ref, watch } from 'vue'; // Added by JO
import { useAuthStore } from './stores/auth';
import AppSidebar from './components/AppSidebar.vue';
import AppHeader from './components/AppHeader.vue';
import AppFooter from './components/AppFooter.vue';
//import { initUserSocket, disconnectUserSocket } from './utils/socket';  // Added by JO

const authStore = useAuthStore();
const isSidebarOpen = ref(false);

function toggleSidebar() {
  isSidebarOpen.value = !isSidebarOpen.value;
}

function closeSidebar() {
  isSidebarOpen.value = false;
}

// Added by JO
// watch(() => authStore.user, (newUser) => {
//   if (newUser && authStore.accessToken) {
//     const currentUserId = newUser.user_uuid || newUser.id;
//     console.log('DEBUG watch for usersocket Extracted ID:', currentUserId);
//     const currentToken = authStore.accessToken; 
//     console.log('DEBUG watch for usersocket Extracted currentToken:', currentToken);

//     if (currentUserId && currentToken) {
//       console.log('Presence Tracker starting for:', currentUserId);
//       initUserSocket(currentUserId, currentToken);
//     }
//   } else {
//   // If newUser is null (meaning they logged out or their session expired)
//     console.log('Presence Tracker stopping');
//     disconnectUserSocket();
//   }
// }, { immediate: true });

onMounted(() => {
  if (authStore.accessToken && !authStore.user) {
    console.log('Fetching user profile on boot...');
    authStore.checkSession();
  }
});

</script>

<template>
  <div class="app-shell min-vh-100">
    <AppSidebar :mobile-open="isSidebarOpen" @close="closeSidebar" />

    <main class="app-main d-flex flex-column bg-dark text-light min-vh-100">
      <AppHeader @toggle-sidebar="toggleSidebar" />
      <section class="container-fluid flex-grow-1 overflow-auto py-3">
        <RouterView />
      </section>
      <AppFooter />
    </main>
  </div>
</template>

<style scoped>
.app-shell {
  display: flex;
  min-width: 0;
  overflow-x: hidden;
}

.app-main {
  flex: 1;
  min-width: 0;
  overflow-x: hidden;
}
</style>
