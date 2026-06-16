import express from 'express';
import authCtrl from './controllers.js'

const router = express.Router();

router.get('/health', authCtrl.health);
router.post('/create_token', authCtrl.create_token);
router.post('/replace_token', authCtrl.replace_token);
router.delete('/delete_token', authCtrl.delete_token);

export default router;