import express from 'express';
import usersCtrl from '../controllers/usersCtrl.js'
import friendsRoutes from "./friendsRoutes.js";
import { decodeJWT } from "../middlewares.js"

const router = express.Router();

router.use('/friends', friendsRoutes);

router.get('/health', usersCtrl.health);
router.get('/me', decodeJWT, usersCtrl.getMe);
router.patch('/me', decodeJWT, usersCtrl.patchMe);
router.post('/create_user', usersCtrl.createUser);
router.post('/get_user', usersCtrl.getUser);
router.post('/batch_user_info', usersCtrl.batchUserInfo);
router.get('/get_xp', usersCtrl.getXP);
router.patch("/update_xp", usersCtrl.updateXP);

export default router;