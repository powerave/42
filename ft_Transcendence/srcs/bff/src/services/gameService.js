import gameClient from "../clients/gameClient.js";
import usersClient from "../clients/usersClient.js";
import {gameSocket, connections, userToSid, gameRooms} from "../socketManager.js"

class gameService {
	async getHealth() {
		const response = await gameClient.health();
		return response.data;
	}

	async getGamesList() {
		const response = await gameClient.listGames();
		return response.data;
	}

	async postNewGame(NbrPlayers, isSolo, userId) {
		const response = await gameClient.createGames(NbrPlayers, isSolo);
		const gameId = String(response.data.game_id);

		console.log('gameId:', gameId, typeof gameId);
		console.log('userId:', userId, typeof userId);
		console.log('userToSid avant:', userToSid);

		// call join_game so front don't have to do it

		// NOTE : should be the only place where the rooms is created if doesn't exists?
		const sid = userToSid.get(String(userId));

		console.log('sid trouvé:', sid);

		// TODO : join called with create 
		if (sid) {
			if (!gameRooms.has(gameId)) gameRooms.set(gameId, []);
			gameRooms.get(gameId).push(sid);
		}
		gameSocket.emit('message', {
			type: 'JOIN_GAME',
			game_id: Number(response.data.game_id),
			user_id: userId,
		});

		// NOTE : est ce que je dois ajouter des trucs dans data?
		// Est ce la bonne facon de faire??
		return response.data;
	}

	async handleSocketMessage(msg, gameSocket) {
		// NOTE : pour l'instant renvoyer en format brut, enrichir ici (validation, auth, etc)

		gameSocket.emit('message', msg);
	}

	async enrichGameEvent(payload) {
		// NOTE : appeler d'autres services https pour resoudre les uuid en pseudo
		// ex : const user = await userClient.getUser(payload.user_id)
		// return { ...payload, username: user.name}

		// @AUDREY en attente de Clara
		// ICI dans le payload il y a les uuid de plusieurs users, il faut renvoyer: les username et l'URL de l'avatar
		return payload; //pass-through pour l'instant
	}

	async updateXPUser(gameId) {
		const responseGame = await gameClient.getGameData(gameId);
		const playersData = responseGame.data.players;

		if (!playersData)
			throw {
				status: 401,
				message: "No game found"
		}

		const responseUser = await usersClient.updateXP(playersData);

		return responseUser.data;
	}
}

export default new gameService();
