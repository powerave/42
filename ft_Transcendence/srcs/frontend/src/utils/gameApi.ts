// http client for game-related API calls, using fetchWithAuth to handle authentication and token refresh automatically.
import { fetchWithAuth } from './fetchWithAuth';

export async function fetchGameHealth() {
  const response = await fetchWithAuth("/api/game/health");
  if (!response.ok) {
    throw new Error(`Erreur HTTP: ${response.status}`);
  }
  return await response.json();
}

export async function createGameApi(numPlayers: number = 2, userId?: number, isSolo: boolean = false) {
  const response = await fetchWithAuth("/api/game/create", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ nbrPlayers: numPlayers, userId: userId, user_id: userId, isSolo: isSolo }),
    credentials: "include"
  });
  if (!response.ok) {
    throw new Error(`Erreur HTTP: ${response.status}`);
  }
  return await response.json();
}

export async function listGamesApi() {
  const response = await fetchWithAuth("/api/game/lists");
  if (!response.ok) {
    throw new Error(`Erreur HTTP: ${response.status}`);
  }
  return await response.json();
}

export async function fetchGameState(gameId: number) {
  const response = await fetchWithAuth(`/api/game/${gameId}/state`);
  if (!response.ok) {
    throw new Error(`Erreur HTTP: ${response.status}`);
  }
  return await response.json();
}

export async function fetchPlayerHand(gameId: number, playerId: number) {
  const response = await fetchWithAuth(`/api/game/${gameId}/hand/${playerId}`);
  if (!response.ok) {
    throw new Error(`Erreur HTTP: ${response.status}`);
  }
  return await response.json();
}
