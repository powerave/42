import tokenClient from "../clients/tokenClient.js";
import usersClient from "../clients/usersClient.js";

class authService {
	async getHealth() {
		const response = await tokenClient.health();
		return response.data;
	}

	async getLogin(identifiers) {

		const response_uuid = await usersClient.getUser(identifiers);
		const userUuid = response_uuid.data.user_id;

		if (!userUuid)
			throw {
				status: 401,
				message: "No userUUID"
		}

		// Create response with token
		const response_token = await tokenClient.createTokens(userUuid);

		// Add avatar URL to response
		response_token.data.avatar_url = response_uuid.data.avatar;

		// Add xp info to response
		const response_xp = await usersClient.getUserXP(userUuid);
		response_token.data.user_xp = response_xp.data.user_xp;

		return response_token.data;
	}

	async getRefresh(refreshToken) {
		const response = await tokenClient.replaceTokens(refreshToken);
		return response.data;
	}

	async getLogout(refreshToken) {
		const response = await tokenClient.deleteTokens(refreshToken);
		return response.data;
	}
}

export default new authService();