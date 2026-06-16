import jwt from "jsonwebtoken";

export async function generateToken(userUuid, secret, expireTime) {
	const token = jwt.sign(
		{
			userId: userUuid
		},
		secret,
		{ 
			algorithm: process.env.ENCRYPTION_ALGO,
			expiresIn: expireTime //change this for debug Jo
		}
	);
	return token;
};