import gameService from "../services/gameService.js";

const health = async (req, res, next) => {
	
	try {
		const data = await gameService.getHealth();
		res.json(data);
	} catch (err) {
		next(err);
	}
};

const lists = async (req, res, next) => {
	
	try {
		const data = await gameService.getGamesList();
		res.json(data);
	} catch (err) {
		next(err);
	}
};

const create = async (req, res, next) => {
	

	try {
		console.log('user_uuid:', req.userUuid);
		const { nbrPlayers } = req.body;
		const { isSolo } = req.body;
		const userId = req.userUuid;
		const data = await gameService.postNewGame(nbrPlayers, isSolo, userId);
		res.json(data);
	} catch (err) {
		next(err);
	}
};

// const updateXP = async (req, res, next) => {
//
// 	try {
// 		const { gameId } = req.body;
// 		const data = await gameService.updateXPUser(gameId);
// 		res.json(data);
// 	} catch (err) {
// 		next(err);
// 	}
// };

export default {
	health,
	lists,
	create,
	// updateXP,
};
