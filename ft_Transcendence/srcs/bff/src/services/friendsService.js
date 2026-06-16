import usersClient from "../clients/usersClient.js";
import { mapUserProfile } from '../utils/userMapper.js';

class friendsService {
	async sendFriendRequest(token, friendEmail) {
		const response = await usersClient.sendFriendRequest(token, friendEmail);
		return response;
		//const rawUser = response.data?.data || response.data;
		//if (!rawUser || !rawUser.user_uuid) {
       // 	throw new Error("Invalid payload: Missing user_uuid from microservice response");
		//}

    	//return mapUserProfile(rawUser);
	}

	async acceptFriend(token, friendUuid) {
		const response = await usersClient.acceptFriend(token, friendUuid);
		return response;
	}

	async getFriendsList(token) {
		const response = await usersClient.getFriendsList(token);
		return (response.data);
	}
}

export default new friendsService();