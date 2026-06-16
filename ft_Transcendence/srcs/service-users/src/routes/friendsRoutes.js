import express from 'express';
import friendsCtrl from '../controllers/friendsCtrl.js'
import { decodeJWT } from "../middlewares.js"

const router = express.Router();

router.get('/', decodeJWT, friendsCtrl.getFriendsList);
router.patch('/accept', decodeJWT, friendsCtrl.acceptRequest);
router.post('/request', decodeJWT, friendsCtrl.sendRequest);

export default router;