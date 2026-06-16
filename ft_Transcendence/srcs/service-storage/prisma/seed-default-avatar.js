import { PrismaClient } from '@prisma/client';

const prisma = new PrismaClient();

async function main() {

    const defaultAvatar = await prisma.upload.upsert({
        where: { file_path: 'AVATAR/default_avatar.jpg' },
        update: {}, 
        create: {
        original_name: 'default_avatar.jpg',
        file_path: 'AVATAR/default_avatar.jpg',
        file_type: 'AVATAR',
        owner_uuid: 'SYSTEM' // Links perfectly to the user created above
        }
    });
    console.log("Default avatar db entry seeded successfully:", defaultAvatar);
}

main()
  .catch((e) => {
    console.error("Default avatar eeding failed:", e);
    process.exit(1);
  })
  .finally(async () => {
    await prisma.$disconnect();
  });