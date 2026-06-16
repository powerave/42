<script setup lang="ts">
import { ref } from 'vue';
import { useRouter } from 'vue-router';
import { useAuthStore } from '../stores/auth';
import type { ApiAuthResponse, LoginPayload } from '../types/auth';

const authStore = useAuthStore();
const router = useRouter();
const email = ref('');
const password = ref('');
const showPassword = ref(false);
const busy = ref(false);
const feedback = ref('');
const feedbackType = ref<'info' | 'success' | 'danger'>('info');
const resultText = ref('');

function setFeedback(message: string, type: 'info' | 'success' | 'danger') {
  feedback.value = message;
  feedbackType.value = type;
}

async function submitLogin() {
  busy.value = true;
  setFeedback('Envoi vers le service…', 'info');
  resultText.value = '';

  const payload: LoginPayload = {
    email: email.value.trim(),
    password: password.value
  };

  try {
    const response = await fetch('/api/auth/login', {
      method: 'POST',
      credentials: 'include',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(payload)
    });
	
    const contentType = response.headers.get('content-type') || '';
    const result: ApiAuthResponse | string = contentType.includes('application/json')
      ? await response.json()
      : await response.text();

    resultText.value = typeof result === 'string' ? result : JSON.stringify(result, null, 2);

    if (!response.ok || (typeof result !== 'string' && result.success === false)) {
      const message = typeof result !== 'string' && result.error
        ? result.error
        : `Erreur HTTP ${response.status}`;
      setFeedback(message, 'danger');
      return;
    }

	localStorage.setItem('isConnected', "1");

    if (typeof result !== 'string' && (result.accessToken || result.token || result.access_token)) {
      authStore.setAccessToken(result.accessToken || result.token || result.access_token);
    }

    setFeedback(typeof result !== 'string' && result.message ? result.message : 'Connexion réussie', 'success');

    if (authStore.accessToken) {
      resultText.value += '\n\n✅ JWT Access Token stocké avec succès dans Pinia et LocalStorage !';
    }

    await authStore.checkSession();
    await router.push({ name: 'home' });
  } catch (error) {
    const message = error instanceof Error ? error.message : 'Erreur inconnue';
    setFeedback(`Erreur: ${message}`, 'danger');
    resultText.value = '';
  } finally {
    busy.value = false;
  }
}
</script>

<template>
  <div class="row justify-content-center g-3 align-items-stretch">
    <div class="col-12 col-xl-6 d-flex">
      <div class="card auth-card bg-dark text-white border border-info w-100">
        <div class="card-header bg-info text-dark fw-bold text-uppercase py-3">{{ $t('auth.login_title') }}</div>
        <div class="card-body p-4 p-xl-5 d-flex flex-column gap-4">
          <div>
            <h2 class="h3 fw-bold mb-2">{{ $t('auth.login_resume') }}</h2>
            <p class="text-secondary mb-0">{{ $t('auth.login_desc') }}</p>
          </div>

          <form class="d-flex flex-column gap-3" @submit.prevent="submitLogin">
            <div>
              <label for="email" class="form-label fw-semibold">{{ $t('auth.email') }}</label>
              <input id="email" v-model="email" name="email" type="email" class="form-control form-control-lg auth-input" placeholder="player@uno.dev" autocomplete="email" required>
            </div>

            <div>
              <label for="password" class="form-label fw-semibold">{{ $t('auth.password') }}</label>
              <div class="input-group input-group-lg">
                <input
                  id="password"
                  v-model="password"
                  name="password"
                  :type="showPassword ? 'text' : 'password'"
                  class="form-control auth-input"
                  placeholder="••••••••"
                  autocomplete="current-password"
                  required
                >
                <button class="btn btn-outline-info fw-bold" type="button" @click="showPassword = !showPassword">
                  {{ showPassword ? $t('auth.hide') : $t('auth.show') }}
                </button>
              </div>
            </div>

            <div class="d-flex flex-wrap gap-2 pt-2">
              <button type="submit" class="btn btn-success btn-lg fw-bold text-uppercase flex-grow-1" :disabled="busy">{{ $t('auth.submit_login') }}</button>
              <button type="reset" class="btn btn-outline-light btn-lg fw-bold text-uppercase" :disabled="busy" @click="email = ''; password = ''">{{ $t('auth.clear') }}</button>
            </div>
          </form>

          <div class="d-flex align-items-center justify-content-between flex-wrap gap-2 pt-2 border-top border-secondary">
            <span class="text-secondary">{{ $t('auth.no_account') }}</span>
            <a class="btn btn-outline-info fw-bold" href="#/signup">{{ $t('auth.go_signup') }}</a>
          </div>

          <div v-if="feedback" class="auth-feedback alert mb-0" :class="`alert-${feedbackType}`" role="status">{{ feedback }}</div>
          <!-- <pre v-if="resultText" class="auth-result mb-0 rounded-4 border border-secondary bg-black text-light p-3">{{ resultText }}</pre> -->
        </div>
      </div>
    </div>
  </div>
</template>
