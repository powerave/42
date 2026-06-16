<script setup lang="ts">
import { ref } from 'vue';
import { useRouter } from 'vue-router';
import type { ApiAuthResponse, SignupPayload } from '../types/auth';

const router = useRouter();

const firstName = ref('');
const lastName = ref('');
const username = ref('');
const email = ref('');
const password = ref('');
const showPassword = ref(false);
const busy = ref(false);
const feedback = ref('');
const feedbackType = ref<'info' | 'success' | 'danger'>('info');

function setFeedback(message: string, type: 'info' | 'success' | 'danger') {
  feedback.value = message;
  feedbackType.value = type;
}

function normalizeSignupMessage(message: string) {
  const trimmedMessage = message.trim();

  if (!trimmedMessage) {
    return '';
  }

  const loweredMessage = trimmedMessage.toLowerCase();

  if (loweredMessage.includes('all fields are required')) {
    return 'Tous les champs sont obligatoires.';
  }

  if (loweredMessage.includes('user already exists')) {
    return "Un compte avec cet email existe déjà. Connecte-toi.";
  }

  if (loweredMessage.includes('user created successfully')) {
    return 'Compte créé avec succès.';
  }

  if (loweredMessage.includes('unique constraint failed')) {
    return "Ces informations sont déjà utilisées. Vérifie l'email ou le pseudo.";
  }

  if (loweredMessage.includes('failed to fetch')) {
    return "Impossible de contacter le service d'inscription.";
  }

  return trimmedMessage;
}

function getSignupResponseMessage(response: Response, result: ApiAuthResponse | string) {
  if (typeof result === 'string') {
    return normalizeSignupMessage(result) || `Erreur HTTP ${response.status}`;
  }

  if (response.ok && result.success !== false) {
    return normalizeSignupMessage(result.message || 'Compte créé avec succès.') || 'Compte créé avec succès.';
  }

  if (response.status === 400) {
    return 'Tous les champs sont obligatoires.';
  }

  if (response.status === 409) {
    return "Un compte avec cet email existe déjà. Connecte-toi.";
  }

  if (response.status >= 500) {
    return "Le service d'inscription rencontre une erreur. Réessaie plus tard.";
  }

  return normalizeSignupMessage(result.message || `Erreur HTTP ${response.status}`) || `Erreur HTTP ${response.status}`;
}

async function submitSignup() {
  busy.value = true;
  setFeedback('Envoi vers le service…', 'info');

  const payload: SignupPayload = {
    first_name: firstName.value.trim(),
    last_name: lastName.value.trim(),
    username: username.value.trim(),
    email: email.value.trim(),
    password: password.value
  };

  try {
    const response = await fetch('/api/profile/signup', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(payload)
    });

    const contentType = response.headers.get('content-type') || '';
    const result: ApiAuthResponse | string = contentType.includes('application/json')
      ? await response.json()
      : await response.text();

    if (!response.ok || (typeof result !== 'string' && result.success === false)) {
      setFeedback(getSignupResponseMessage(response, result), 'danger');
      return;
    }

    setFeedback(getSignupResponseMessage(response, result), 'success');
    window.setTimeout(() => {
      router.push('/login');
    }, 700);
  } catch (error) {
    const message = error instanceof Error ? normalizeSignupMessage(error.message) : 'Impossible de créer le compte.';
    setFeedback(message || 'Impossible de créer le compte.', 'danger');
  } finally {
    busy.value = false;
  }
}

function resetForm() {
  firstName.value = '';
  lastName.value = '';
  username.value = '';
  email.value = '';
  password.value = '';
}
</script>

<template>
  <div class="row justify-content-center g-3 align-items-stretch">
    <div class="col-12 col-xl-6 d-flex">
      <div class="card auth-card bg-dark text-white border border-info w-100">
        <div class="card-header bg-info text-dark fw-bold text-uppercase py-3">{{ $t('auth.signup_title') }}</div>
        <div class="card-body p-4 p-xl-5 d-flex flex-column gap-4">
          <div>
            <h2 class="h3 fw-bold mb-2">{{ $t('signup.join_table') }}</h2>
            <p class="text-secondary mb-0">{{ $t('signup.fill_profile') }}</p>
          </div>

          <form class="d-flex flex-column gap-3" @submit.prevent="submitSignup">
            <div class="row g-3">
              <div class="col-12 col-md-6">
                <label for="first_name" class="form-label fw-semibold">{{ $t('auth.firstname') }}</label>
                <input id="first_name" v-model="firstName" name="first_name" type="text" class="form-control form-control-lg auth-input" :placeholder="$t('signup.placeholder_firstname')" autocomplete="given-name" required>
              </div>
              <div class="col-12 col-md-6">
                <label for="last_name" class="form-label fw-semibold">{{ $t('auth.lastname') }}</label>
                <input id="last_name" v-model="lastName" name="last_name" type="text" class="form-control form-control-lg auth-input" :placeholder="$t('signup.placeholder_lastname')" autocomplete="family-name" required>
              </div>
              <div class="col-12 col-md-6">
                <label for="username" class="form-label fw-semibold">{{ $t('auth.username') }}</label>
                <input id="username" v-model="username" name="username" type="text" class="form-control form-control-lg auth-input" :placeholder="$t('signup.placeholder_username')" autocomplete="username" required>
              </div>
              <div class="col-12 col-md-6">
                <label for="email" class="form-label fw-semibold">{{ $t('auth.email') }}</label>
                <input id="email" v-model="email" name="email" type="email" class="form-control form-control-lg auth-input" :placeholder="$t('signup.placeholder_email')" autocomplete="email" required>
              </div>
              <div class="col-12">
                <label for="password" class="form-label fw-semibold">{{ $t('auth.password') }}</label>
                <div class="input-group input-group-lg">
                  <input
                    id="password"
                    v-model="password"
                    name="password"
                    :type="showPassword ? 'text' : 'password'"
                    class="form-control auth-input"
                    placeholder="••••••••"
                    autocomplete="new-password"
                    minlength="8"
                    required
                  >
                  <button class="btn btn-outline-info fw-bold" type="button" @click="showPassword = !showPassword">
                    {{ showPassword ? $t('auth.hide') : $t('auth.show') }}
                  </button>
                </div>
                <div class="form-text text-secondary">{{ $t('signup.password_hint') }}</div>
              </div>
            </div>

            <div class="d-flex flex-wrap gap-2 pt-2">
              <button type="submit" class="btn btn-success btn-lg fw-bold text-uppercase flex-grow-1" :disabled="busy">{{ $t('auth.submit_signup') }}</button>
              <button type="reset" class="btn btn-outline-light btn-lg fw-bold text-uppercase" :disabled="busy" @click="resetForm">{{ $t('auth.clear') }}</button>
            </div>
          </form>

          <div class="d-flex align-items-center justify-content-between flex-wrap gap-2 pt-2 border-top border-secondary">
            <span class="text-secondary">{{ $t('auth.has_account') }}</span>
            <RouterLink class="btn btn-outline-info fw-bold" to="/login">{{ $t('auth.go_login') }}</RouterLink>
          </div>

          <div v-if="feedback" class="auth-feedback alert mb-0" :class="`alert-${feedbackType}`" role="status">{{ feedback }}</div>
        </div>
      </div>
    </div>
  </div>
</template>
