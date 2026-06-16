import express from 'express';
import profileCtrl from '../controllers/profileCtrl.js';
import { authMiddleware } from "../middlewares/authMiddleware.js"

const router = express.Router();

router.get('/', authMiddleware, profileCtrl.me);
router.get('/health', profileCtrl.health);
router.post('/signup', profileCtrl.signup);
router.patch('/', authMiddleware, profileCtrl.patchMe);
router.get('/avatar', authMiddleware, profileCtrl.avatarGallery);
router.post('/avatar', authMiddleware, profileCtrl.uploadAvatar);
router.get('/my_history', authMiddleware, profileCtrl.myHistory);

export default router;