const { createApp, nextTick } = Vue;
const { createRouter, createWebHashHistory } = VueRouter;

function updateSidebarActive(route) {
    document.querySelectorAll('.sidebar-item').forEach((item) => {
        const isActive = item.dataset.route === route;
        item.classList.toggle('active', isActive);
        item.classList.toggle('bg-primary', isActive);
    });
}

function updateHeaderUserState() {
    const token = localStorage.getItem('auth_token');
    const headerLink = document.getElementById('header-user-link');
    const headerLabel = document.getElementById('header-user-label');

    if (!headerLink || !headerLabel) {
        return;
    }

    if (token) {
        headerLabel.textContent = 'Mon profil';
        headerLink.setAttribute('href', '#/profile');
    } else {
        headerLabel.textContent = "S'identifier";
        headerLink.setAttribute('href', '#/login');
    }
}

function templateContent(templateId) {
    const template = document.getElementById(templateId);
    const fallback = document.getElementById('view-not-found');
    return (template || fallback)?.innerHTML || '<div></div>';
}

const HomeView = { template: templateContent('view-home') };
const ProfileView = { template: templateContent('view-profile') };
const LeaderboardView = { template: templateContent('view-leaderboard') };
const FriendsView = { template: templateContent('view-friends') };
const LoginView = { template: templateContent('view-login') };
const SignupView = { template: templateContent('view-signup') };
const PrivacyView = { template: templateContent('view-privacy') };
const TermsView = { template: templateContent('view-terms') };
const NotFoundView = { template: templateContent('view-not-found') };

const router = createRouter({
    history: createWebHashHistory(),
    routes: [
        { path: '/', redirect: '/home' },
        { path: '/home', name: 'home', component: HomeView },
        { path: '/profile', name: 'profile', component: ProfileView },
        { path: '/leaderboard', name: 'leaderboard', component: LeaderboardView },
        { path: '/friends', name: 'friends', component: FriendsView },
        { path: '/login', name: 'login', component: LoginView },
        { path: '/signup', name: 'signup', component: SignupView },
        { path: '/privacy', name: 'privacy', component: PrivacyView },
        { path: '/terms', name: 'terms', component: TermsView },
        { path: '/:pathMatch(.*)*', name: 'not-found', component: NotFoundView }
    ]
});

function syncUiWithRoute(routeName) {
    updateSidebarActive(routeName);
    updateHeaderUserState();

    nextTick(() => {
        if (window.authApp && typeof window.authApp.initAuthForms === 'function') {
            window.authApp.initAuthForms();
        }
    });
}

const App = {
    template: '<router-view />',
    mounted() {
        syncUiWithRoute(this.$route.name);
    },
    watch: {
        $route(to) {
            syncUiWithRoute(to.name);
        }
    }
};

window.addEventListener('DOMContentLoaded', () => {
    createApp(App).use(router).mount('#spa-view');
});
