// Imported from node.js
import jwt from "jsonwebtoken";
import bcrypt from "bcrypt";

// Import from files
import tokenRepositery from "./repository.js"
import { generateToken } from "./utils.js"

class AuthService {
	async getHealth() {
		return {
			status: 200,
			message: 'All good :)'
		}
	}

	async getTokenCreated(userUuid) {
		const accessToken = await generateToken(userUuid, process.env.JWT_ACCESS_SECRET, process.env.TOKEN_ACC_EXP);
		const refreshToken = await generateToken(userUuid, process.env.JWT_REFRESH_SECRET, '2h');

		const decodeRefreshToken = jwt.decode(refreshToken);
		const refreshTokenData = {
			user_uuid: decodeRefreshToken.userId,
			refresh_token_hash: await bcrypt.hash(refreshToken, 10),
			expires_at: new Date(decodeRefreshToken.exp * 1000),
			created_at: new Date(decodeRefreshToken.iat * 1000)
		};

		await tokenRepositery.create(refreshTokenData);

		return {
			status: 200,
			message: "Tokens on their way",
			user_id: userUuid,
			accessToken: accessToken,
			refreshToken: refreshToken
		}
	}

	async getTokenReplaced(refreshToken) {
		const verified = jwt.verify(refreshToken, process.env.JWT_REFRESH_SECRET);
		const exists = await tokenRepositery.check(verified.userId);

		if (!exists)
			throw {
				status: 403,
				message: "Invalid token"
		};

		const matching = await bcrypt.compare(refreshToken, exists.refresh_token_hash);

		if (!matching)
			throw {
				status: 403,
				message: "Invalid token"
		};

		const newAccessToken = await generateToken(verified.userId, process.env.JWT_ACCESS_SECRET, process.env.TOKEN_ACC_EXP);
		const newRefreshToken = await generateToken(verified.userId, process.env.JWT_REFRESH_SECRET, '2h');

		const decodeRefreshToken = jwt.decode(newRefreshToken);
		const refreshTokenData = {
			user_uuid: decodeRefreshToken.userId,
			refresh_token_hash: await bcrypt.hash(newRefreshToken, 10),
			expires_at: new Date(decodeRefreshToken.exp * 1000),
			created_at: new Date(decodeRefreshToken.iat * 1000)
		};
		
		await tokenRepositery.update(refreshTokenData);

		return {
			status: 200,
			message: "Tokens renewed",
			accessToken: newAccessToken,
			refreshToken: newRefreshToken
		}
	};

	async getTokenDeleted(refreshToken) {
		const payload = jwt.decode(refreshToken);

		await tokenRepositery.delete(payload.userId);

		return {
			status: 200,
			message: "Refresh Token deleted"
		}
	}
}

export default new AuthService();