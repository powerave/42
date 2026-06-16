import express from 'express';
import friendsCtrl from '../controllers/friendsCtrl.js';
import { authMiddleware } from "../middlewares/authMiddleware.js"

const router = express.Router();

router.get('/', authMiddleware, friendsCtrl.handleFriendsList);
router.post('/request', authMiddleware, friendsCtrl.handleAddFriend);
router.patch('/accept', authMiddleware, friendsCtrl.handleAcceptFriend);

export default router;