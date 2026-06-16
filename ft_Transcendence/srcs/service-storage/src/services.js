import { S3Client, PutObjectCommand, GetObjectCommand, DeleteObjectCommand } from "@aws-sdk/client-s3";
import { getSignedUrl } from "@aws-sdk/s3-request-presigner";
import { v4 as uuidv4 } from 'uuid';
import path from 'path';
import { PrismaClient } from '@prisma/client';
import { s3 } from './scripts/s3Client.js';

const prisma = new PrismaClient();
 
//init MiniIo
// const s3 = new S3Client({
//     endpoint: `http://service-minio:${process.env.PORT_MINIO_INT || 9000}`,
//     region: "us-east-1",
//     forcePathStyle: true, //Mandatory for MinIO, otherwise it'll try app-uploads.service-minio:9000
//     credentials: {
//         //both are provided in the .env
//         accessKeyId: process.env.MINIO_ROOT_USER,
//         secretAccessKey: process.env.MINIO_ROOT_PASSWORD
//     }
// });

class StorageService {
    async getHealth() {
        return {
            status: 200,
            message: 'Storage service OK'
        }
    }

    async processAndStoreFile({ file, category, user_uuid }) {

        const ALLOWED_CATEGORIES = ['AVATAR', 'OTHER'];

        if (!ALLOWED_CATEGORIES.includes(category)) {
            const error = new Error(`Invalid category: ${category}`);
            error.name = "INVALID_CATEGORY";
            throw error; //caught by the controller!
        }

        const originalname = file.originalname;
        const filename = `${uuidv4()}${path.extname(originalname)}`;
        const key = `${category}/${user_uuid}/${filename}`;

        let newUpload = null;

        try { //links db entry to minio upload to avoid zombies
            newUpload = await prisma.upload.create({
                data: {
                    owner_uuid: user_uuid,
                    file_path: key,
                    file_type: category,
                    original_name: originalname
                },
                select: { //what will be included in the response
                    id: true,
                    file_path: true,
                    original_name: true
                }
            });
        //save file to MinIO
            await s3.send(new PutObjectCommand({
                Bucket: process.env.MINIO_BUCKET,
                Key: key,
                Body: file.buffer, 
                ContentType: file.mimetype 
            }));

            console.log(`File saved: ${filename} to MinIO in ${category}`);
            //DEBUG
            console.log(`File id: ${newUpload.id}`);

            return (newUpload);

        } catch (error) {
            console.error("File storage transaction failed:", error.message);

            if (newUpload) {
                console.log(`Rolling back database record for upload: ${newUpload.id}`);
                await prisma.upload.delete({
                    where: { id: newUpload.id }
                });
            }
            throw new Error(`File upload processing failed: ${error.message}`);
        }
    }

    async deleteFile({ fileId, userUuid }) {
        try {
            const file = await prisma.upload.findFirst({
            where: {
                id: fileId,
                owner_uuid: userUuid //checks that user us the owner
            },
            select: {
                file_path: true //sends back file_path
            }
        });

        if (!file) {
            throw new Error("Unauthorized or file not found");
        }

        await prisma.upload.delete({
            where: { id: fileId }
        });
        return { file_path: file.file_path };

        //TODO continue with Minio deletion and link the 2 again as a transaction
        //return ();
        }
        catch (error) {}
    }

    async getAvatarGallery(userUuid) {
    //add safety access rights?
        const gallery = await prisma.upload.findMany({
            where: { 
                file_type: 'AVATAR',
                OR: [
                    { owner_uuid: userUuid },
                    { owner_uuid: 'SYSTEM' }
                ] 
            },
            select: {
                id: true,
                file_path: true,
                original_name: true
            },
            orderBy: {
                id: 'desc' // Shows newest uploads first in the UI
            }
        });
        return gallery.map(file => ({
            id: file.id,
            file_path: file.file_path,
            original_name: file.original_name,
        }));
    }
};

export default new StorageService();

export const disconnectDB = async () => {
    if (prisma) {
        await prisma.$disconnect();
    }
};