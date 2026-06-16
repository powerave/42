import express from 'express';
import controller from './controllers.js'
import multer from 'multer';
import { decodeJWT } from "./middlewares.js"

const router = express.Router();

const MAX_SIZE_BYTES = 5 * 1024 * 1024; // 5MB
const ALLOWED_MIMETYPES = [
    'image/jpeg',
    'image/png',
    'application/pdf'
];

router.get('/health', controller.health);

const uploadFile = multer({ 
    storage: multer.memoryStorage(), // Reuses memory buffers for MinIO 
    limits: {
        fileSize: MAX_SIZE_BYTES // Enforces file size at the stream level
    },
    fileFilter: (req, file, cb) => {
        if (ALLOWED_MIMETYPES.includes(file.mimetype)) {
            // Accept the file
            cb(null, true);
        } else {
            // Reject the file with a custom error
            cb(new multer.MulterError('LIMIT_UNEXPECTED_FILE', 'Invalid file type. Only JPEG, PNG, and PDF are allowed.'));
        }
    }
});

//RESTful approach, only differences are the methods
router.post('/', decodeJWT,
            uploadFile.single('file'),
            controller.uploadFile);

router.delete('/', decodeJWT, controller.deleteFile);

router.get('/my_avatars', decodeJWT, controller.myAvatars);

export default router;