import axios from "axios";
import { mtlsAgent } from "../mtls.js"

const PORT_STORAGE = process.env.PORT_STORAGE;

class storageClient {
	constructor() {
		this.https = axios.create({
			baseURL: `https://service-storage:${PORT_STORAGE}/storage`,
			timeout: 3000,
			httpsAgent: mtlsAgent,
		});
	}

	health() {
		return this.https.get("/health");
	}

	createTokens(userUuid) {
		return this.https.post("/create_token", null, {
			headers: {
				"x-user-uuid": userUuid
			}
		});
	}

	myAvatars(token) {
		return this.https.get("/my_avatars", {
				headers: {
					Authorization: `Bearer ${token}`
				}
			}	
		);
	}

	postUploadAvatar(fileStream, token) {

		return this.https.post("/", fileStream, {
				headers: {
					Authorization: `Bearer ${token}`,
					'Content-Type': fileStream.headers['content-type']
				},
				maxContentLength: Infinity,
        		maxBodyLength: Infinity
			}	
		);
	}
	
}

export default new storageClient();
