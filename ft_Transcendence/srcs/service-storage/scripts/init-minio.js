import fs from 'fs';
import { s3 } from './s3Client.js';
import { HeadBucketCommand, CreateBucketCommand, PutBucketPolicyCommand, HeadObjectCommand, PutObjectCommand } from "@aws-sdk/client-s3";

const MINIO_BUCKET = process.env.MINIO_BUCKET;

async function initStorage() {
    try {
        //Verify/Create Bucket
        try {
            await s3.send(new HeadBucketCommand({ Bucket: MINIO_BUCKET }));
        } catch (err) {
            if (err.$metadata?.httpStatusCode === 404) {
                await s3.send(new CreateBucketCommand({ Bucket: MINIO_BUCKET }));
            } else { throw err; }
        }

        //Set Policy
        const policy = {
            Version: "2012-10-17",
            Statement: [{
                Sid: "PublicReadForAvatars",
                Effect: "Allow",
                Principal: "*",
                Action: ["s3:GetObject"],
                Resource: [`arn:aws:s3:::${MINIO_BUCKET}/AVATAR/*`]
            }]
        };
        await s3.send(new PutBucketPolicyCommand({ Bucket: MINIO_BUCKET, Policy: JSON.stringify(policy) }));
        
        //eed Default Avatar
        const defaultKey = "AVATAR/default_avatar.jpg";
        try {
            await s3.send(new HeadObjectCommand({ Bucket: MINIO_BUCKET, Key: defaultKey }));
        } catch (err) {
            const localPath = "./public/default_avatar.jpg";
            if (fs.existsSync(localPath)) {
                await s3.send(new PutObjectCommand({
                    Bucket: MINIO_BUCKET,
                    Key: defaultKey,
                    Body: fs.createReadStream(localPath),
                    ContentType: "image/jpeg"
                }));
            }
        }
        console.log("MinIO storage successfully initialized.");
    } catch (err) {
        console.error("Initialization script failed:", err);
        process.exit(1);
    }
}

// Automatically runs when called via the shell entrypoint
initStorage();