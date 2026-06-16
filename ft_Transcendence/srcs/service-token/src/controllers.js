import authService from "./services.js"

const health = async (req, res, next) => {
	try {
		const data = await authService.getHealth();
		res.json(data);
	} catch (err) {
		next(err);
	}
};

const create_token = async (req, res, next) => {
	try {
		const data = await authService.getTokenCreated(req.user_uuid);
		res.json(data);
	} catch (err) {
		next(err);
	}
};

const replace_token = async (req, res, next) => {

	try {
		const refreshToken = req.body.refreshToken;

		if (!refreshToken) {
			const error = new Error("Missing refresh token");
			error.status = 400;

			throw error;
		}

		const data = await authService.getTokenReplaced(refreshToken);

		res.json(data);

	} catch (err) {
		next (err);
	}
};

const delete_token = async (req, res, next) => {

	try {
		const refreshToken = req.body.refreshToken;

		if (!refreshToken) {
			const error = new Error("Missing refresh token");
			error.status = 400;

			throw error;
		}

		const data = await authService.getTokenDeleted(refreshToken);
		res.json(data);
		
	} catch (err) {
		next(err);
	}
};

export default {
	health,
	create_token,
	replace_token,
	delete_token,
};