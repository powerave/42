import express from "express";
import authRoutes from "./routes.js";
import cookieParser from "cookie-parser";

import { errorHandler } from "./middlewares.js"

const app = express();

// CORS middleware setup //TO REVIEW TO BE MORE SECURE, ex: not allowing everyone
app.use((req, res, next) => {
	res.header('Access-Control-Allow-Origin', '*'); // Allows requests from any website.
	// res.header('Access-Control-Allow-Credentials', 'true');
	res.header('Access-Control-Allow-Methods', 'GET, POST'); // Specifies which HTTP methods are allowed.
	res.header(
		'Access-Control-Allow-Headers', 'Content-Type, x-user-uuid'
	); // Enables the Content-Type header, which is useful for sending JSON with fetch.
	
	req.user_uuid = req.headers['x-user-uuid'];
	
	console.log(req.method, req.url); // DEBUGGING
	next();
});

app.use(express.json()); //to read JSON body
app.use(cookieParser()); //to read cookies

app.use('/auth', authRoutes);

app.use(errorHandler);

export default app;