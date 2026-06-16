import friendRepository from "../repositories/friendRepository.js"

class FriendsService {
    async sendFriendRequest(requesterUuid, friendEmail) {

        const friend = await friendRepository.getUuid(friendEmail);

        if (!friend) {
            throw {
				status: 404,
				message: "User not found."
            }
        }

        if (requesterUuid === friend.user_uuid) {
            throw {
				status: 400,
				message: "You cannot send a friend request to yourself."
            }
        }

        const alreadyFriends = await friendRepository.getFriendship(requesterUuid, friend.user_uuid);

        if (alreadyFriends) {
            throw {
				status: 400,
				message: "You are already friends."
            }
        }

        return await friendRepository.createFriendship(requesterUuid, friend.user_uuid);
    }

    async acceptFriendRequest(receiverUuid, requesterUuid) {

        const pendingRequest = await friendRepository.checkPendingRequest(requesterUuid, receiverUuid);

        if (!pendingRequest || pendingRequest.status !== 'PENDING') {
            throw {
				status: 401,
				message: "No pending friend request found."
            }
        }

        return await friendRepository.acceptFriend(pendingRequest.id);
    }

    //getter sends back map (like for avatars) of the requester's friends
    async getFriendsList(userUuid) {
        const friendships = await friendRepository.establishedFriendships(userUuid);

        //the following presorts the object that is sent back. These are the already established friendships
        //but we don't care who started the friendship, it doessn't matter if the requester started the friendship or just confirmed it
        //just sends back the friend's profile, could be the receiver or requester of the friendship initially 
        return friendships.map(f => {
            return f.requester_uuid === userUuid ? f.receiver : f.requester;
        }); //BFF can select the values the front actually needs and stitch the online status into it via Redis / socket
    }
}

export default new FriendsService();