import usersClient from "../clients/usersClient.js";
import storageClient from "../clients/storageClient.js";
import gameClient from "../clients/gameClient.js";
import { mapUserProfile } from '../utils/userMapper.js';

class profileService {

	async getHealth(accessToken) {
		const response = await usersClient.health(accessToken);
		return response.data;
	}

	async getCreateUser(userData) {
		const response = await usersClient.createUser(userData);
		return response.data;
	}

	async getMyProfile(token) {
		const response = await usersClient.myProfile(token);
		return response;
	}

	async patchMyProfile(bodyData, token) {
		const response = await usersClient.patchMyProfile(bodyData, token);
		return (response.data);
	}

	async getMyAvatars(token) {
		const response = await storageClient.myAvatars(token);
		return response.data;
		//TODO add mapping function
	}

	async postUploadAvatar(fileStream, token) {
		return await storageClient.postUploadAvatar(fileStream, token);
	}

	async getMyHistory(userUUID) {
		const responseGame =  await gameClient.playerHistory(userUUID);

		if (!responseGame.data)
			throw {
				status: 401,
				message: "No game data"
		}

		const batchUserId = [
			...new Set(
				responseGame.data.flatMap(game =>
					game.players.map(player => player.user_id)
				)
			)
		];


		const responseUser = await usersClient.batchUserInfo(batchUserId);
		
		if (!responseUser.data)
			throw {
				status: 401,
				message: "No corresponding users"
		}

		const enrichedGames = responseGame.data.map(game => ({
			...game,
			players: game.players.map(player => ({
				result: player.result,
				score: player.score,
				user: responseUser.data[player.user_id] ?? null
			}))
		}));

		return enrichedGames;
	}
}

export default new profileService();