import { PrismaClient } from '@prisma/client';
import bcrypt from 'bcryptjs';

const prisma = new PrismaClient();

async function main() { 
// console.log("Storage database seeding for default avatar...");

//   const systemUser = await prisma.user.upsert({
//     where: { user_uuid: 'SYSTEM' },
//     update: {}, //Only if it doesn't already exist
//     create: {
//       user_uuid: 'SYSTEM',
//       username: 'default-avatar',
//       avatar: 'AVATAR/default_avatar.jpg'
//     }
//   });
  console.log('Seeding test users into database...');

  // Hash a standard development password ('password123')
    const saltRounds = 10;
    const defaultHashedPassword = await bcrypt.hash('password123', saltRounds);

    // Create complete, valid payloads
    const testUsers = [
        {
            user_uuid: process.env.T1_UUID,
            email: 'john@test.fr',
            first_name: 'John',
            last_name: 'Test',
            username: 'john_test',
            password: defaultHashedPassword, // Stored securely as a hash
            avatar: 'AVATAR/default_avatar.jpg',
            xp: 1000
        },
        {
            user_uuid: process.env.T2_UUID,
            email: 'karen@test.fr',
            first_name: 'Karen',
            last_name: 'Test',
            username: 'karen_test',
            password: defaultHashedPassword,
            avatar: 'AVATAR/default_avatar.jpg',
            xp: 500
        },
        {
            user_uuid: process.env.T3_UUID,
            email: 'george@test.fr',
            first_name: 'George',
            last_name: 'Test',
            username: 'george_test',
            password: defaultHashedPassword,
            avatar: 'AVATAR/default_avatar.jpg',
            xp: 42
        },
        {
            user_uuid: process.env.T4_UUID,
            email: 'julia@test.fr',
            first_name: 'Julia',
            last_name: 'Test',
            username: 'julia_test',
            password: defaultHashedPassword,
            avatar: 'AVATAR/default_avatar.jpg',
            xp: 24
        }
    ];

    // 2. Safely upsert your records
    for (const user of testUsers) {
        await prisma.user.upsert({
            where: { user_uuid: user.user_uuid },
            update: {}, 
            create: user,
        });
        console.log(`Test user created: ${user.username} (${user.email})`);
    }

    console.log('Test users seeded successfully.');
}

main()
    .then(async () => {
        await prisma.$disconnect();
    })
    .catch(async (e) => {
        console.error('Seeding test users failed:', e);
        await prisma.$disconnect();
        process.exit(1);
    });


