import authService from "../services/authService.js";

const health = async (req, res, next) => {
	try {
		const data = await authService.getHealth();
		res.json(data);
	} catch (err) {
		next(err);
	}
};

const login = async (req, res, next) => {
	const identifiers = req.body;

	if (!identifiers) {
		return res.status(400).json({
			error: "All fields required"
		})
	}

	try {
		const data = await authService.getLogin(identifiers);

		req.session.auth = {
			user: data.user_id,
			accessToken: data.accessToken,
			refreshToken: data.refreshToken
		};

		const minioUrl = `${process.env.MINIO_PUBLIC_URL}`;

		return res.status(data.status).json({
			message: data.message,
			avatar_url: `${minioUrl}/${data.avatar_url}`,
			user_xp: data.user_xp
		});
	} catch (err) {
		next(err);
	}
};

const logout = async (req, res, next) => {
	// console.log("Entering logout controller");
	try {
		const refreshToken = req.session?.auth?.refreshToken;

		if (refreshToken) {
			await authService.getLogout(refreshToken);
		}

		req.session.destroy((err) => {
		  if (err) {
			return next(err);
		  }

		  res.clearCookie("connect.sid", {
			httpOnly: true,
			secure: true,
			sameSite: "Strict"
		  });

		  return res.status(200).json({
			success: true
		  });
		});
	} catch (err) {
		next(err);
	}
};

export default {
	health,
	login,
	logout,
};
