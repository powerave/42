import leaderboardService from "../services/leaderboardService.js";

const allStats = async (req, res, next) => {
	try {
		const data = await leaderboardService.getAllStats();
		res.json(data);
	} catch (err) {
		next(err);
	}
};

export default {
	allStats,
};