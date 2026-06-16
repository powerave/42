import axios from "axios";
import { mtlsAgent } from "../mtls.js"

const port = process.env.PORT_USERS || 3000; 

class usersClient {
	constructor() {
		this.https = axios.create({
			baseURL: `https://service-users:${port}/users`,
			timeout: 3000,
			httpsAgent: mtlsAgent,
		});
	}

	health(accessToken) {
		return this.https.get("/health",
			{
				headers: {
					Authorization: `Bearer ${accessToken}`
				}
			}			
		);
	}

	createUser(userData) {
		return this.https.post("/create_user", userData);
	}

	getUser(identifiers) {
		return this.https.post("/get_user", identifiers);
	}

	myProfile(token) {
		return this.https.get('/me', {
				headers: {
					Authorization: `Bearer ${token}`
				}
			}	
		);
	}

	patchMyProfile(bodyData, token) {
		return this.https.patch("/me", bodyData, {
				headers: {
					Authorization: `Bearer ${token}`
				}
			}	
		);
	}

	sendFriendRequest(token, friendEmail) {
    	return this.https.post("/friends/request", 
        	{ friendEmail }, 
			{
				headers: {
					Authorization: `Bearer ${token}`,
				}
			}
		);
	}
	
	acceptFriend(token, friendUuid) {
    	return this.https.patch('/friends/accept', 
        	{ friendUuid }, 
			{
				headers: {
					Authorization: `Bearer ${token}`,
				}
			}
		);
	}

	getFriendsList(token) {
    	return this.https.get('/friends', 
			{
				headers: {
					Authorization: `Bearer ${token}`,
				}
			}
		);
	}

	batchUserInfo(batchUser) {
		return this.https.post("/batch_user_info", batchUser);
	}
	
	acceptFriend(token, friendUuid) {
    	return this.https.patch('/friends/accept', 
        	{ friendUuid }, 
			{
				headers: {
					Authorization: `Bearer ${token}`,
				}
			}
		);
	}

	getFriendsList(token) {
    	return this.https.get('/friends', 
			{
				headers: {
					Authorization: `Bearer ${token}`,
				}
			}
		);
	}

	getUserXP(userUUID) {
		return this.https.get("/get_xp", {
			headers: {
				'x-user-uuid': userUUID,
			},
		});
	}

	updateXP(usersScore) {
		return this.https.patch("/update_xp", usersScore);
	}
}

export default new usersClient();
