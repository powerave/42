import fs from 'fs';
import https from 'https';
import { S3Client } from "@aws-sdk/client-s3";
import { NodeHttpHandler } from "@smithy/node-http-handler";

const caCert = fs.readFileSync('/run/secrets/ca_crt');

export const s3 = new S3Client({
    endpoint: `https://minio:${process.env.PORT_MINIO_INT}`, 
    region: "us-east-1",   
    forcePathStyle: true, 
    credentials: {
        accessKeyId: process.env.MINIO_ROOT_USER,
        secretAccessKey: process.env.MINIO_ROOT_PASSWORD 
    },
    requestHandler: new NodeHttpHandler({
        httpsAgent: new https.Agent({
            ca: caCert,
            rejectUnauthorized: true 
        })
    })
});