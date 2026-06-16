import { useAuthStore } from '../stores/auth';

/**
 * fetch wrapper that automatically handles token refresh on 401 responses, and retries the original request with the new token.
*/
export async function fetchWithAuth(input: RequestInfo | URL, init?: RequestInit): Promise<Response> {
  const authStore = useAuthStore();
  
  // prepare headers with the current access token (if available)
  const headers = new Headers(init?.headers);

  // initial request with the current token
  const config = { ...init, headers };
  let response = await fetch(input, config);

  if (response.status === 401) {
    // console.warn('Erreur 401: Token potentiellement expiré. Tentative de rafraîchissement...');
    
    const refreshSuccess = await authStore.refresh();
    
    if (refreshSuccess && authStore.accessToken) {
      const newConfig = { ...init, headers };
      response = await fetch(input, newConfig);
    } else {
      // console.error('Échec du rafraîchissement. L\'utilisateur doit se reconnecter.');
      // window.location.href = '/login';
    }
  }

  return response;
}