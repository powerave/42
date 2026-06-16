import axios from "axios";
import { mtlsAgent, mtlsOptions } from "../mtls.js"

class gameClient {
	constructor() {
		this.https = axios.create({
			baseURL: "https://service-game:8000/api/game",
			timeout: 3000,
			httpsAgent: mtlsAgent,
		});
		this._socket = null; //sera injecte par socketManager
	}

	//Appele une seule fois depuis socketManager apres la connexion
	setSocket(socket) {
		this._socket = socket;
	}

	emitGameSocket(envent, data) {
		if (!this._socket) throw new Error('gameSocket not initialised');

		this._socket.emit(event, data);
	}

	health() {
		return this.https.get("/health");
	}

	listGames() {
		 return this.https.get("/listGames");
	}

	createGames(numberOfPlayers, isSolo) {
		return this.https.post("/create", {
			num_players: numberOfPlayers,
			solo: isSolo
		});
	}

	gameStats() {
		return this.https.get("/stats");
	}

	playerStats(userUUID) {
		return this.https.get("/playerStats", {
			params: {
				userId: userUUID
			}
		});
	}

	playerHistory(userUUID) {
		return this.https.get("/playerHistory", {
			params: {
				userId: userUUID
			}
		});
	}

	getGameData(gameId) {
		return this.https.get("/gameData", {
			params: {
				gameId: gameId
			}
		});
	}
}

export default new gameClient();
