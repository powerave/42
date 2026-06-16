import profileService from "../services/profileService.js";
import { mapUserProfile } from '../utils/userMapper.js';

const health = async (req, res, next) => {
    try {
        const data = await authService.getHealth();
        res.json(data);
    } catch (err) {
        next(err);
    }
};

const signup = async (req, res, next) => {
	try {
		const userData = req.body;
		const data = await profileService.getCreateUser(userData);
		res.json(data);
	} catch (err) {
		next(err);
	}
};

const me = async (req, res, next) => {//TO BE TESTED
    try {
		const accessToken = req.accessToken;
		if (!accessToken) {
            return res.status(401).json({ 
                error: "Unauthorized: Missing JWT accessToken" 
            });
        }
        const response = await profileService.getMyProfile(accessToken);

        const rawUser = response.data?.data || response.data;
        const profileData = mapUserProfile(rawUser);

        if (!profileData) {
            return res.status(502).json({ error: "Invalid payload received from users microservice" });
        }

        return res.status(200).json(profileData);
    } catch (err) {
        next(err);
    }
};

const patchMe = async (req, res, next) => {
    try {
		const accessToken = req.accessToken;
		if (!accessToken) {
            return res.status(401).json({ 
                error: "Unauthorized: Missing JWT accessToken" 
            });
        }
        const updatedData = await profileService.patchMyProfile(req.body, accessToken);  
        
        const raw = updatedData.data?.data || updatedData.data || updatedData;
        const mappedMe = mapUserProfile(raw);      

        return res.status(200).json(mappedMe);
    } catch (err) {
        // if (err.response) {
        //     // Serror code and payload message returned by microservice
        //     return res.status(err.response.status).json({
        //         error: err.response.data?.error || err.response.data?.message || err.message 
        //     });
        // }
        next(err);
    }
};

const avatarGallery = async (req, res, next) => {//TO BE TESTED
    try {
		const accessToken = req.accessToken;
		if (!accessToken) {
            return res.status(401).json({ 
                error: "Unauthorized: Missing JWT accessToken" 
            });
        }
      
        const [me, gallery] = await Promise.all([
            profileService.getMyProfile(accessToken),
            profileService.getMyAvatars(accessToken)
        ]);
        console.log('gallery avatargallery: ', gallery);
        console.log('gallery.data avatargallery: ', gallery.data);

        //const minioUrl = `${process.env.MINIO_PUBLIC_URL}:${process.env.PORT_MINIO_INT}/${process.env.MINIO_BUCKET}`;
        const minioUrl = `${process.env.MINIO_PUBLIC_URL}`;
        const current = me.avatar;

        const stitchedResponse = gallery.map(file => ({
            ...file,
            is_current: file.file_path === current,
            fullUrl: file.file_path ? `${minioUrl}/${file.file_path}` : null
        }));
        console.log('stitchedresponse avatargallery: ', stitchedResponse);
    
        return res.status(200).json(stitchedResponse);
        
    } catch (err) {
        if (err.response) {
         // Serror code and payload message returned by microservice
            return res.status(err.response.status).json({
                error: err.response.data?.error || err.response.data?.message || err.message 
            });
        }
        next(err);
    }
};


const uploadAvatar = async (req, res, next) => {//TO BE TESTED
    try {
		const accessToken = req.accessToken;
		if (!accessToken) {
            return res.status(401).json({ 
                error: "Unauthorized: Missing JWT accessToken" 
            });
        }
        const fileStream = req;
        const response = await profileService.postUploadAvatar(fileStream, accessToken);  
        const minioUrl = process.env.MINIO_PUBLIC_URL; 
        const newImg = {
            id: response.data.id,
            name: response.data.original_name,
            key: response.data.key,
            url: `${minioUrl}/${response.data.key}`
        }; 
        return res.status(201).json(newImg);
    } catch (err) {
        if (err.response) {
            // Serror code and payload message returned by microservice
            return res.status(err.response.status).json({
                error: err.response.data?.error || err.response.data?.message || err.message 
            });
        }
        next(err);
    }
};

const myHistory = async (req, res, next) => {
    try {
        const data = await profileService.getMyHistory(req.userUuid);
        res.json(data);
    } catch (err) {
        next(err);
    }
};

export default {
    health,
	signup,
	me,
    patchMe,
    avatarGallery,
    uploadAvatar,
    myHistory
};
