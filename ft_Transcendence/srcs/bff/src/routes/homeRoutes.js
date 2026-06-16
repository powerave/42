import express from 'express';
import homeCtrl from '../controllers/homeCtrl.js';
import { authMiddleware } from '../middlewares/authMiddleware.js';

const router = express.Router();

router.get('/refresh_xp', authMiddleware, homeCtrl.refreshXP);
router.get('/my_stats', authMiddleware, homeCtrl.myStats);

export default router;