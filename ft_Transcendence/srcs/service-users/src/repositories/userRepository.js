import { prisma } from '../app.js';

class UserRepository {
	async create(userData) {
		return prisma.user.create({
			data: userData
		});
	};

	async checkEmail(userEmail) {
		return prisma.user.count({
			where: {
				email: userEmail,
			}
		})
	};

	async get(userEmail) {
		return prisma.user.findUnique({
			where: {
				email: userEmail,
			},
		});
	};

	async getMultiple(batch) {
		return prisma.user.findMany({
			where: {
				user_uuid: {
					in: batch
				}
			},
			select: {
				user_uuid: true,
				username:true,
				avatar: true,
				xp: true
			}
		})
	};

	async getByUUID(userUUID) {
		return prisma.user.findUnique({
			where: {
				user_uuid: userUUID,
			},
		});
	};

	async getUser(userUuid) {
		return prisma.user.findUnique({
            where: { user_uuid: userUuid },
            select: {
                user_uuid: true,
                username: true,
                avatar: true,
                first_name: true,
                last_name: true,
                email: true,
                xp: true,
            }                           
        });
	};

	async updateUser(userUuid, updatedData) {
		return prisma.user.update({
            where: { user_uuid: userUuid },
            data: updatedData,
            select: {
                user_uuid: true,
                username: true,
                first_name: true,
                last_name: true,
                email: true,
                avatar: true
            }
        });
	};

	async incrementMultipleXP(usersScores) {
		return prisma.$transaction(
			usersScores.map(({ user_id, score }) =>
				prisma.user.update({
					where: {
						user_uuid: user_id
					},
					data: {
						xp: {
							increment: Math.abs(score)
						}
					}
				})
			)
		);
	}
}

export default new UserRepository();
