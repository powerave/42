const forms = {
    loginForm: {
        endpoint: '/api/auth/login',
        successRedirect: '#/home',
        successLabel: 'Connexion réussie',
        buildPayload: () => ({
            email: document.getElementById('email')?.value?.trim(),
            password: document.getElementById('password')?.value
        })
    },
    signupForm: {
        endpoint: '/api/profile/signup',
        successRedirect: '#/login',
        successLabel: 'Inscription réussie',
        buildPayload: () => ({
            first_name: document.getElementById('first_name')?.value?.trim(),
            last_name: document.getElementById('last_name')?.value?.trim(),
            username: document.getElementById('username')?.value?.trim(),
            email: document.getElementById('email')?.value?.trim(),
            password: document.getElementById('password')?.value
        })
    }
};

function setFeedback(message, type = 'info') {
    const box = document.getElementById('msg');
    if (!box) {
        return;
    }

    box.className = `auth-feedback alert alert-${type} mb-0`;
    box.textContent = message;
    box.classList.remove('d-none');
}

function setResult(value) {
    const box = document.getElementById('result');
    if (!box) {
        return;
    }

    if (typeof value === 'undefined' || value === null || value === '') {
        box.textContent = '';
        box.classList.add('d-none');
        return;
    }

    box.textContent = typeof value === 'string' ? value : JSON.stringify(value, null, 2);
    box.classList.remove('d-none');
}

function setFormBusy(form, busy) {
    form.querySelectorAll('input, button').forEach((element) => {
        element.disabled = busy;
    });
}

function navigateTo(redirect) {
    if (!redirect) {
        return;
    }

    if (redirect.startsWith('#/')) {
        if (window.location.pathname.endsWith('/index.html') || window.location.pathname.endsWith('/')) {
            window.location.hash = redirect;
            return;
        }
        window.location.href = `./index.html${redirect}`;
        return;
    }

    window.location.href = redirect;
}

async function parseResponse(response) {
    const contentType = response.headers.get('content-type') || '';

    if (contentType.includes('application/json')) {
        return response.json();
    }

    return response.text();
}

function initAuthForms() {
    Object.entries(forms).forEach(([formId, config]) => {
        const form = document.getElementById(formId);

        if (!form || form.dataset.authBound === '1') {
            return;
        }

        form.dataset.authBound = '1';

        form.addEventListener('submit', async (event) => {
            event.preventDefault();

            const endpoint = form.dataset.endpoint || config.endpoint;
            const redirect = form.dataset.redirect || config.successRedirect;
            const payload = config.buildPayload();

            setFeedback('Envoi vers le service…', 'info');
            setResult('');
            setFormBusy(form, true);

            try {
                const response = await fetch(endpoint, {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json'
                    },
                    body: JSON.stringify(payload)
                });

                const result = await parseResponse(response);

                setResult(result);

                if (!response.ok || (result && result.success === false)) {
                    const message = (result && result.message) ? result.message : `Erreur HTTP ${response.status}`;
                    setFeedback(message, 'danger');
                    return;
                }

                setFeedback((result && result.message) || config.successLabel, 'success');

                if (result && typeof result === 'object' && result.token) {
                    localStorage.setItem('auth_token', result.token);
                }

                if (redirect) {
                    window.setTimeout(() => {
                        navigateTo(redirect);
                    }, 900);
                }
            } catch (error) {
                setFeedback(`Erreur: ${error.message}`, 'danger');
                setResult('');
            } finally {
                setFormBusy(form, false);
            }
        });
    });
}

window.authApp = {
    initAuthForms
};

document.addEventListener('DOMContentLoaded', initAuthForms);