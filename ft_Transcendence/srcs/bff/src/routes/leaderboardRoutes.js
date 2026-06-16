import express from 'express';
import leaderboardCtrl from '../controllers/leaderboardCtrl.js';

const router = express.Router();

router.get('/all_stats', leaderboardCtrl.allStats);

export default router;