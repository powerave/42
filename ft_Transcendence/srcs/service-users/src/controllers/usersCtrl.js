import usersService from "../services/usersService.js"

const health = async (req, res, next) => {
    console.log("Test header authorization", req.headers.authorization); // pour check transmission JWT si besoin
  
    try {
        const data = await usersService.getHealth();
        res.json(data);
    } catch (err) {
        next(err);
    }
};

const patchMe = async (req, res, next) => {
     try {
        const userUuid = req.userUuid;
        
        if (!userUuid) {
            const error = new Error("Missing user uuid");
            error.status = 400; 
            throw error;        
        }

        const data = await usersService.patchMyProfile(userUuid, req.body);

        return res.status(200).json(data);

    } catch (err) {
        next(err);
    }
};

const getMe = async (req, res, next) => {
    try {
        const userUuid = req.userUuid;
        if (!userUuid) {
            const error = new Error("Missing user uuid");
            error.status = 400; 
            throw error;        
        }
        const me = await usersService.getMyProfile(userUuid);

        return res.status(200).json(me);

    } catch (err) {
        next(err);
    }
};

const createUser = async (req, res, next) => {
    
	try {
        const newUserData = req.body;
        
    	if (!newUserData) {
            const error = new Error("Missing user information");
            error.status = 400;
            
            throw error;
    	}

		const data = await usersService.getCreateUser(newUserData);
        
        return res.status(data.status).json({
            message: data.message
        });
	} catch (err) {
		next(err);
	}
};

const getUser = async (req, res, next) => {

    try {
        const identifiers = req.body;

        if (!identifiers) {
            const error = new Error("All fields are required");
            error.status = 400;
            
            throw error;
        }

        const data = await usersService.getGetUser(identifiers);
        res.json(data);
    } catch (err) {
        next (err);
    }
};

const batchUserInfo = async (req, res, next) => {

    try {
        const batch_user = req.body;

        if (!batch_user) {
            const error = new Error("Missing batch users info");
            error.status = 400;

            throw error;
        }

        const data = await usersService.getBatchUserInfo(batch_user);
        res.json(data);
    } catch (err) {
        next (err);
    }
};

const getXP = async (req, res, next) => {

    try {
        const user_uuid = req.headers['x-user-uuid'];

        if (!user_uuid) {
            const error = new Error("Missing user UUID");
            error.status = 400;

            throw error;
        }

        const data = await usersService.getUserXP(user_uuid);
        res.json(data);
    } catch (err) {
        next (err);
    }
};

const updateXP = async (req, res, next) => {
    try {
        const usersScores = req.body;

        if (!usersScores){
            const error = new Error("Missing info on scores");
            error.status = 400;

            throw error;
        }

        const data = await usersService.getUpdateXP(usersScores);
        res.json(data)
    } catch (err) {
        next (err);
    }
};

export default {
    health,
    getMe,
    patchMe,
    createUser,
    getUser,
    batchUserInfo,
    getXP,
    updateXP,
};