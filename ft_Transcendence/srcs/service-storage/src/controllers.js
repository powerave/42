import storageService from "./services.js"
import multer from 'multer';

const health = async (req, res, next) => {
    try {
        const data = await storageService.getHealth();
        res.json(data);
    } catch (err) {
        next(err);
    }
};

const uploadFile = async (req, res, next) => {
    try {
        // was file actually uploaded by multer?
        if (!req.file) {
            return res.status(400).json({ error: 'No file uploaded.' });
        }
        const mimetype = req.file?.mimetype;

        let category;

        if (mimetype && /^image\/(jpeg|png|jpg)$/.test(mimetype)) {
            category = "AVATAR";
        } else {
            category = "OTHER"; 
        }

        const user_uuid = req.userUuid;

        if (!user_uuid) {
            const error = new Error("Unauthorized: No user identifier provided.");
            error.name = "UNAUTHORIZED";
            return next(error);
        }
        const uploadResult = await storageService.processAndStoreFile({
            file: req.file,
            category,
            user_uuid
        });

        return res.status(200).json({
            message: 'Upload successful',
            key: uploadResult.file_path,
            original_name: uploadResult.original_name,
            id: uploadResult.id
        });
    } catch (err) {
        next(err);
    }
};

//getfile for upload bonus?

//front has to send id
const deleteFile = async (req, res, next) => {
    try {
        // same as in upload
        const userUuid = req.userUuid;
        if (!userUuid) {
            const error = new Error("Missing user uuid");
            error.status = 400; 
            throw error;        
        }
         // if we change for  BigInt, change this to BigInt(req.params.id)
        const fileId = parseInt(req.params.id, 10); 
        
        if (isNaN(fileId)) {
            return res.status(400).json({ error: "Invalid file id." });
        }
        //test if user has the right to delete file
        const result = await storageService.deleteFile({ fileId, userUuid });
        return res.status(200).json(result);

    } catch (err) {
        if (err.status) {
            return res.status(err.status).json({ error: err.message });
        }
        next(err);
    }
};

const myAvatars = async (req, res, next) => {
    try {
        const userUuid = req.userUuid;
        if (!userUuid) {
            const error = new Error("Missing user uuid");
            error.status = 400; 
            throw error;        
        }
        const gallery = await storageService.getAvatarGallery(userUuid);
        res.json(gallery);
    } catch (err) {
        next(err); // This 'err' is correct because it's populated by the catch block
    }
};

export default {
    health,
    uploadFile,
    deleteFile,
    myAvatars
};