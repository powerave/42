import jwt from "jsonwebtoken";
import authService from "../services/authService.js";

export async function authMiddleware(req, res, next) {

	if (!req.session.auth) {
		return res.status(401).json({
			error: "Not logged"
		});
	}
	
	const accessToken = req.session.auth.accessToken;

	try {
		const verified = jwt.verify(accessToken, process.env.JWT_ACCESS_SECRET);

		req.accessToken = accessToken;
		req.userUuid = verified.userId;
		
		return next();

	} catch (err) {

		if (err.name === 'TokenExpiredError') {
			
			try {
				const refreshToken = req.session.auth.refreshToken;

				const data = await authService.getRefresh(refreshToken);

				req.session.auth = {
					user: data.user_id,
					accessToken: data.accessToken,
					refreshToken: data.refreshToken
				};

				req.accessToken = data.accessToken;
				
				return next();

			} catch (refreshErr) {

				console.error(refreshErr);

				req.session.destroy(() => {});

				return res.status(401).json({
					error: "Session expired"
				});
			}			
		};

		return res.status(403).json ({
			message: "Forbidden",
			error: "Invalid token"
		});

	}
};
