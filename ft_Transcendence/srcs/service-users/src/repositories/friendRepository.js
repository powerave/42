import { prisma } from '../app.js';

class FriendRepository {
	async getFriendship(requesterUuid, receiverUuid) {
		return prisma.friendship.findFirst({
            where: {
                OR: [
                    { requester_uuid: requesterUuid, receiver_uuid: receiverUuid },
                    { requester_uuid: receiverUuid, receiver_uuid: requesterUuid }
                ]
            }
        });
	};

	async createFriendship(requesterUuid, receiverUuid) {
		await prisma.friendship.create({
            data: {
                requester_uuid: requesterUuid,
                receiver_uuid: receiverUuid,
               // status: 'PENDING' //see the schema prisma for the possible states
                status: 'ACCEPTED'
            }
        });

        const newFriend = await prisma.user.findUnique({
        where: { 
            user_uuid: receiverUuid 
            },
            select: {
                user_uuid: true,
                username: true,
                avatar: true
            }
        });

        return newFriend;
	};

	async checkPendingRequest(requesterUuid, receiverUuid) {
		return await prisma.friendship.findUnique({
            where: {
                requester_uuid_receiver_uuid: {
                    requester_uuid: requesterUuid,
                    receiver_uuid: receiverUuid
                }
            }
        });
	};

    async getUuid(friendEmail) {
		return prisma.user.findUnique({
			where: {
				email: friendEmail,
			},
            select: {
                user_uuid: true
            }
		});
	};

	async acceptFriend(id) { //TEST
		prisma.friendship.update({
            where: {
                id: id
            },
            data: {
                status: 'ACCEPTED'
            }
        });
	}

	async establishedFriendships(userUuid) {
		return prisma.friendship.findMany({
            where: {
                status: 'ACCEPTED',
                OR: [
                    { requester_uuid: userUuid },
                    { receiver_uuid: userUuid }
                ]
            },
            //since the tables are linked, the values from the users table can also be selected
            include: {
                requester: {
                    select: { user_uuid: true, username: true, avatar: true }
                },
                receiver: {
                    select: { user_uuid: true, username: true, avatar: true }
                }
            }
        });
	}
}

export default new FriendRepository();