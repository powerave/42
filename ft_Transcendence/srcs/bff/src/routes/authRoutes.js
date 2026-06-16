import express from 'express';
import authCtrl from '../controllers/authCtrl.js';
import { authMiddleware } from '../middlewares/authMiddleware.js';

const router = express.Router();

router.get('/health', authCtrl.health);
router.post('/login', authCtrl.login);
router.delete('/logout', authCtrl.logout);
// router.post('/logout', authCtrl.logout);

export default router;
