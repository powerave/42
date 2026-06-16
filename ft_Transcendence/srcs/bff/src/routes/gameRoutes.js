import express from 'express';
import gameCtrl from '../controllers/gameCtrl.js';
import { authMiddleware } from "../middlewares/authMiddleware.js"

const router = express.Router();

router.get('/health', gameCtrl.health);
router.get('/lists', gameCtrl.lists);
// router.post('/update_xp_after_game', gameCtrl.updateXP);
router.post('/create', authMiddleware, gameCtrl.create);
// router.post('/create', gameCtrl.create);

export default router;
