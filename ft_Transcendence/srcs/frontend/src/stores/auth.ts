import { ref } from 'vue';
import { defineStore } from 'pinia';
import { initSocket, disconnectSocket } from '../utils/socket';

export const useAuthStore = defineStore('auth', () => {
  const isAuthenticated = ref<boolean>(false);
  const user = ref<any>(null);
  const isLoading = ref<boolean>(true);
  const accessToken = ref<string | null>(localStorage.getItem('accessToken') || null);
  let refreshRequest: Promise<boolean> | null = null;

  function setAccessToken(token: string) {
    accessToken.value = token;
    localStorage.setItem('accessToken', token);
  }

  function setAuthenticatedUser(value: any) {
    isAuthenticated.value = true;
    user.value = value;
  }

  function clearAccessToken() {
    accessToken.value = null;
    localStorage.removeItem('accessToken');
  }

  function buildAuthHeaders(baseHeaders?: HeadersInit) {
    const headers = new Headers(baseHeaders);

    return headers;
  }


  async function checkSession() {
    isLoading.value = true;
    try {
      // const response = await authFetch('/api/auth/me');

	  const isCon = localStorage.getItem('isConnected');
	  //console.log("la vriable :", isCon);
	  if (isCon === null) {
        isAuthenticated.value = false;
        user.value = null;
		//console.log("yo");
        return;
	  }
      const response = await authFetch('/api/profile');

      if (!response.ok) {
        isAuthenticated.value = false;
        user.value = null;
        return;
      }

      const result = await response.json();
      setAuthenticatedUser(result);
      initSocket(result.id, accessToken.value);
    } catch (error) {
      //console.error('Error checking session:', error);
      isAuthenticated.value = false;
      user.value = null;
    } finally {
      isLoading.value = false;
    }
  }

  async function logout() {
    try {
      const headers: Record<string, string> = {
        'Content-Type': 'application/json'
      };

      await fetch('/api/auth/logout', {
        method: 'DELETE',
        headers,
        credentials: 'include'
      });
    } catch (error) {
      //console.error('Error logging out:', error);
    } finally {
      isAuthenticated.value = false;
      user.value = null;
	  localStorage.removeItem('isConnected');
      clearAccessToken();
      disconnectSocket();
    }
  }

//   async function refresh(): Promise<boolean> {
//     if (refreshRequest) {
//       return refreshRequest;
//     }

//     refreshRequest = (async () => {
//       try {
//         const response = await fetch('/api/auth/refresh', {
//           method: 'POST',
//           credentials: 'include'
//         });

//         if (!response.ok) {
//           clearAccessToken();
//           isAuthenticated.value = false;
//           user.value = null;
//           return false;
//         }

//         const result = await response.json();
//         const token = result?.accessToken || result?.token || result?.access_token;

//         if (!token || typeof token !== 'string') {
//           clearAccessToken();
//           isAuthenticated.value = false;
//           user.value = null;
//           return false;
//         }

//         setAccessToken(token);
//         isAuthenticated.value = true;
//         initSocket(user.value?.id, token);
//         return true;
//       } catch (error) {
//         //console.error('Error refreshing token:', error);
//         clearAccessToken();
//         isAuthenticated.value = false;
//         user.value = null;
//         return false;
//       } finally {
//         refreshRequest = null;
//       }
//     })();

//     return refreshRequest;
//   }

  async function authFetch(input: RequestInfo | URL, init: RequestInit = {}, retryOnUnauthorized = true) {
    const requestInit: RequestInit = {
      ...init,
      credentials: init.credentials ?? 'include',
      headers: buildAuthHeaders(init.headers)
    };

    const response = await fetch(input, requestInit);

    if (response.status !== 401 || !retryOnUnauthorized) {
      return response;
    }

    // const refreshed = await refresh();
    // if (!refreshed) {
    //   return response;
    // }

    return fetch(input, {
      ...init,
      credentials: init.credentials ?? 'include',
      headers: buildAuthHeaders(init.headers)
    });
  }

  return {
    isAuthenticated,
    user,
    isLoading,
    accessToken,
    setAccessToken,
    setAuthenticatedUser,
    clearAccessToken,
    checkSession,
    logout,
	// refresh,
    authFetch
  };
});