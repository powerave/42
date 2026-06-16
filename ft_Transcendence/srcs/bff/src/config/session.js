import session from "express-session";
import { RedisStore } from "connect-redis";

import { redisClient } from "./redis.js";

export const sessionMiddleware = session({

	store: new RedisStore({
		client: redisClient,
		prefix: "bff:"
	}),

	secret: process.env.REDIS_SECRET,
	resave: false,
	saveUninitialized: false,

	cookie: {
      httpOnly: true,
      secure: true,
      sameSite: "Strict",
      maxAge: 1000 * 60 * 60 * 24 //24h
    }
});