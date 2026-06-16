import gameClient from "../clients/gameClient.js";
import usersClient from "../clients/usersClient.js"

class homeService {
	async getRefreshXP(userUUID) {
		const response = await usersClient.getUserXP(userUUID);
		return response.data;
	}

	async getMyStats(userUUID) {
		const response = await gameClient.playerStats(userUUID);
		return response.data;
	}
}

export default new homeService();