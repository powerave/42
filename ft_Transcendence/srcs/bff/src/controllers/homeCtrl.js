import homeService from "../services/homeService.js"

const refreshXP = async (req, res, next) => {

	try {
		const data = await homeService.getRefreshXP(req.userUuid);
		res.json(data);
	} catch (err) {
		next(err);
	}
};

const myStats = async (req, res, next) => {

	try {
		const data =  await homeService.getMyStats(req.userUuid);
		res.json(data);
	} catch (err) {
		next(err);
	}
};

export default {
	refreshXP,
	myStats,
};