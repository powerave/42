import friendsService from "../services/friendsService.js"

const sendRequest = async (req, res, next) => {
    try {
         const myUuid = req.userUuid;
        if (!myUuid) {
            const error = new Error("Missing user uuid");
            error.status = 400; 
            throw error;        
        }

        const { friendEmail } = req.body;
        if (!friendEmail) {
            return res.status(400).json({ error: "Send request (friendsCtrl): Missing username in request body" });
        }
        
        const friendreq = await friendsService.sendFriendRequest(myUuid, friendEmail);
        return res.status(201).json(friendreq);
    } catch (err) {
        next(err);
    }
};

//not included
const acceptRequest = async (req, res, next) => {
    
    try {
        const myUuid = req.userUuid;
        if (!myUuid) {
            const error = new Error("Missing user uuid");
            error.status = 400; 
            throw error;        
        }

        const { friendUuid } = req.body;
        if (!friendUuid) {
            const error = new Error("Missing user uuid in request body");
            error.status = 400; 
            throw error;  
        }

        const newFriendship = await friendsService.acceptFriendRequest(myUuid, friendUuid);
        
        return res.status(200).json(newFriendship);
    } catch (err) {
        next(err);
    }
};

const getFriendsList = async (req, res, next) => {

    try {
        const userUuid = req.userUuid;
        if (!userUuid) {
            const error = new Error("Missing user uuid");
            error.status = 400; 
            throw error;        
        }
        const friendsList = await friendsService.getFriendsList(userUuid);
        res.json(friendsList);
    } catch (err) {
        next (err);
    }
};


export default {
    acceptRequest,
    getFriendsList,
    sendRequest
};