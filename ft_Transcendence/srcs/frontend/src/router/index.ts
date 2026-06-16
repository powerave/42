import { createRouter, createWebHashHistory, type RouteLocationNormalizedLoaded } from 'vue-router';
import HomeView from '../views/HomeView.vue';
import ProfileView from '../views/ProfileView.vue';
import LeaderboardView from '../views/LeaderboardView.vue';
import FriendsView from '../views/FriendsView.vue';
import LoginView from '../views/LoginView.vue';
import SignupView from '../views/SignupView.vue';
import PlayView from '../views/PlayView.vue';
import PrivacyView from '../views/PrivacyView.vue';
import TermsView from '../views/TermsView.vue';
import NotFoundView from '../views/NotFoundView.vue';
import { useAuthStore } from '../stores/auth';

const router = createRouter({
  history: createWebHashHistory(),
  routes: [
    { path: '/', redirect: '/home' },
    { path: '/home', name: 'home', component: HomeView },
    { path: '/profile', name: 'profile', component: ProfileView, meta: { requiresAuth: true } },
    { path: '/leaderboard', name: 'leaderboard', component: LeaderboardView },
    { path: '/friends', name: 'friends', component: FriendsView, meta: { requiresAuth: true } },
    { path: '/login', name: 'login', component: LoginView },
    { path: '/signup', name: 'signup', component: SignupView },
    { path: '/privacy', name: 'privacy', component: PrivacyView },
    { path: '/play/:id', name: 'play', component: PlayView, meta: { requiresAuth: true } },
    { path: '/terms', name: 'terms', component: TermsView },
    { path: '/:pathMatch(.*)*', name: 'not-found', component: NotFoundView }
  ]
});

let isCheckingSession = false;

router.beforeEach(async (to: RouteLocationNormalizedLoaded) => {
  const authStore = useAuthStore();

  if (!isCheckingSession) {
    isCheckingSession = true;
    await authStore.checkSession();
  }
  if (to.meta.requiresAuth && !authStore.isAuthenticated) {
    return { name: 'login', query: { redirect: to.fullPath } };
  }

  if (to.name === 'login' && authStore.isAuthenticated) {
    return { name: 'home' };
  }

  return true;
});

export default router;
