import axios from "axios";
import { mtlsAgent } from "../mtls.js"

class tokenClient {
	constructor() {
		this.https = axios.create({
			baseURL: "https://service-token:3002/auth",
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

	replaceTokens(refreshToken) {
		return this.https.post("/replace_token", {
			refreshToken: refreshToken
		});
	}

	deleteTokens(refreshToken) {
		return this.https.delete("/delete_token", {
			data: {
				refreshToken: refreshToken
			}
		});
	}
}

export default new tokenClient();
