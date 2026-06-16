import friendsService from "../services/friendsService.js";
import { mapUserProfile } from '../utils/userMapper.js';
import { redisClient } from "../config/redis.js";

const handleAddFriend = async (req, res, next) => {
	try {
		const accessToken = req.accessToken;
		if (!accessToken) {
            return res.status(401).json({ 
                error: "Unauthorized: Missing JWT accessToken" 
            });
        }

		const { friendEmail } = req.body; // Has to be sent by frontend in the JSON body (Max and JM)

        if (!friendEmail) {
            return res.status(400).json({ error: "E-mail required" });
        }

		const response = await friendsService.sendFriendRequest(accessToken, friendEmail);

        const rawUser = response.data?.data || response.data;

        if (!rawUser) {
            return res.status(502).json({ error: "Invalid payload received from users microservice" });
        }

        const friendUuid = rawUser.user_uuid;
        
        const presenceStatus = await redisClient.get(`presence:${friendUuid}`);
        console.log('presenceStatus :', presenceStatus);
        const frontendStatus = (presenceStatus === "ONLINE") ? "online" : "offline";
        
        //const minioUrl = `${process.env.MINIO_PUBLIC_URL}:${process.env.PORT_MINIO_INT}/${process.env.MINIO_BUCKET}`;
        const minioUrl = `${process.env.MINIO_PUBLIC_URL}`;

        //const mappedFriend = mapUserProfile(rawUser);
        return res.status(201).json({ 
                id: friendUuid, 
                username: rawUser.username, 
                avatar: `${minioUrl}/${rawUser.avatar}`, 
                status: frontendStatus 
        });
    } catch (err) {
        if (err.response) {
            return res.status(err.response.status).json({ error: err.response.data?.error || err.message });
        }
        next(err);
    }
};


const handleAcceptFriend = async (req, res, next) => {
	try {
		const accessToken = req.accessToken;
		if (!accessToken) {
            return res.status(401).json({ 
                error: "Unauthorized: Missing JWT accessToken" 
            });
        }

		const { friendUuid } = req.body; // Has to be sent by frontend in the JSON body (Max and JM)

        if (!friendUuid) {
            return res.status(400).json({ error: "Target user UUID required" });
        }
		
		const response = await friendsService.acceptFriend(accessToken, friendUuid);
	
        return res.status(200).json({ 
            message: "Friend request accepted", 
            data: response.data 
        });
    } catch (err) {
        if (err.response) {
            return res.status(err.response.status).json({ error: err.response.data?.error || err.message });
        }
        next(err);
    }
};

const handleFriendsList = async (req, res, next) => {
	try {
        const accessToken = req.accessToken;
		if (!accessToken) {
            return res.status(401).json({ 
                error: "Unauthorized: Missing JWT accessToken" 
            });
        }

        const list = await friendsService.getFriendsList(accessToken);
        //const arrFriends = Array.isArray(list?.data) ? list.data : [];

        // if (arrFriends.length === 0) {
        //     return res.status(200).json([]);
        // }
        const rawFriends = list?.data || list || [];

        //const minioUrl = `${process.env.MINIO_PUBLIC_URL}:${process.env.PORT_MINIO_INT}/${process.env.MINIO_BUCKET}`;
        const minioUrl = `${process.env.MINIO_PUBLIC_URL}`;
        //TODO with redis
        //const friendUuids = rawFriends.map(friend => `presence:${friend.user_uuid}`);
        //const redisStatuses = await connectRedis.mGet(friendUuids);
        // const stitchedList = arrFriends.map((friend, index) => {
        // //const isOnline = redisStatuses[index] === "ONLINE";

        // return {
        //         user_uuid: friend.user_uuid,
        //         username: friend.username,
        //         first_name: friend.first_name,
        //         last_name: friend.last_name,
        //         full_url: `${minioUrl}/${friend.avatar}`,
        //         //is_online: isOnline
        //     };
        // });
        const stitchedList = [];
            for (const friend of rawFriends) {
           // const friendProfile = mapUserProfile(friend); 
            const presenceStatus = await redisClient.get(`presence:${friend.user_uuid}`);
            console.log(`Redis Status for ${friend.username}:`, presenceStatus);            
            const frontendStatus = (presenceStatus === "ONLINE") ? "online" : "offline";
    
            if (friend) {
                stitchedList.push({
                    id: friend.user_uuid, 
                    username: friend.username, 
                    avatar: `${minioUrl}/${friend.avatar}`, 
                    status: frontendStatus 
                });
            }
        }

        return res.status(200).json(stitchedList);

	} catch (err) {
        console.error("Error inside handleFriendsList BFF friendsCtrl:", err.message);
		next(err);
	}
};


export default {
	handleAddFriend,
	handleAcceptFriend,
	handleFriendsList
};
