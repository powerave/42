import express from "express";
import cookieParser from "cookie-parser";

import authRoutes from "./routes/authRoutes.js";
import homeRoutes from "./routes/homeRoutes.js";
import gameRoutes from "./routes/gameRoutes.js";
import profileRoutes from "./routes/profileRoutes.js";
import friendsRoutes from "./routes/friendsRoutes.js";
import leaderboardRoutes from "./routes/leaderboardRoutes.js";

import { sessionMiddleware } from "./config/session.js";
import { errorHandler } from "./middlewares/errorMiddleware.js";

const app = express();

app.use((req, res, next) => {
  res.setHeader('Access-Control-Allow-Origin', '*');
  res.setHeader('Access-Control-Allow-Headers', 'Origin, X-Requested-With, Content, Accept, Content-Type, Authorization');
  res.setHeader('Access-Control-Allow-Methods', 'GET, POST, PUT, DELETE, PATCH, OPTIONS');
 
  req.user_uuid = req.headers['x-user-uuid'];
 
	console.log(req.method, req.url); // DEBUGGING
  
  next();
});

app.use(express.json()); //to read JSON body
app.use(cookieParser()); //to read cookies

app.use(sessionMiddleware); // Redis for cache

app.use('/auth', authRoutes);
app.use('/home', homeRoutes);
app.use('/game', gameRoutes);
app.use('/friends', friendsRoutes);
app.use('/profile', profileRoutes);
app.use('/leaderboard', leaderboardRoutes);


app.use(errorHandler);

export default app;
