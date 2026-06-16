import gameClient from "../clients/gameClient.js";
import usersClient from "../clients/usersClient.js";

class leaderboardService {
	async getAllStats() {
		// Faire l'appel ici du service game pour avoir les statistiques
		const responseGame = await gameClient.gameStats();

		if (!responseGame.data)
			throw {
				status: 401,
				message: "No game data"
		}

		const batchUser = [
			...new Set(
				responseGame.data.map(player => player.user_id)
			)
		]

		const responseUser = await usersClient.batchUserInfo(batchUser);

		if (!responseUser.data)
			throw {
				status: 401,
				message: "No corresponding users"
		}

		const leaderboardData = responseGame.data.map(user  => ({
			user,
			userDetail: responseUser.data[user.user_id]
		}))

		return leaderboardData;
	}
}

export default new leaderboardService();