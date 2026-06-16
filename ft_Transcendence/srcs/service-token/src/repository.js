import { PrismaClient } from "@prisma/client";

const prisma = new PrismaClient();

class tokenRepositery {
	async create(refreshTokenData) {
		return prisma.token.create({
			data: refreshTokenData
		})
	};

	async check(userUuid) {
		return prisma.token.findUnique({
			where: {
				user_uuid: userUuid,
			},
		});
	};

	async update(refreshTokenData) {
		return prisma.token.update({
			where: {
				user_uuid: refreshTokenData.user_uuid,
			},
			data: refreshTokenData
		});
	}

	async delete(userUuid) {
		return prisma.token.delete({
			where: {
				user_uuid: userUuid
			},
		});
	};
};

export default new tokenRepositery();

export const disconnectDB = async () => {
	if (prisma) {
		await prisma.$disconnect();
	}
};