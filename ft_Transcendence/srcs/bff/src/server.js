import https from "https";
import fs from "fs";
import app from './app.js';

import { connectRedis } from "./config/redis.js";
import { Server } from "socket.io";
import { initSocketManager } from './socketManager.js';
import { sessionMiddleware } from './config/session.js';

// Check port value
const normalizePort = val => {
	const port = parseInt(val, 10);

	if (isNaN(port)) {
		return false;
	}
	if (port >= 0 && port <= 65535) {
		return port;
	}
	return false;
};

const port = normalizePort(process.env.PORT);
app.set('port', port);

// Check error
const errorHandler = error => {
	if (error.syscall !== 'listen') {
		throw error;
	}
	const address = server.address();
	const bind = typeof address === 'string' ? 'pipe ' + address : 'port: ' + port; // opérateur ternaire -> condition ? valueIfTrue : valueIfFalse
	switch (error.code) {
		case 'EACCES':
			console.error(bind + ' requires elevated privileges.');
			process.exit(1);
			break;
		case 'EADDRINUSE':
			console.error(bind + ' is already in use.');
			process.exit(1);
			break;
		default:
			throw error;
	}
};

//charging secrets
const tlsOptions = {
	key: fs.readFileSync('/run/secrets/bff_key'),
	cert: fs.readFileSync('/run/secrets/bff_crt'),
	ca: fs.readFileSync('/run/secrets/ca_crt'),
	// requestCert: true,
	// rejectUnauthorized: true, VOIR AVEC CLARA!!!
}

const server = https.createServer(tlsOptions, app);

await connectRedis();
//attach Socket.IO to the https server
const io = new Server(server, { cors: { origin: "*" } });
io.engine.use(sessionMiddleware);
initSocketManager(io);

// .on to listen to events
server.on('error', errorHandler);
server.on('listening', () => {
	const address = server.address();
	const bind = typeof address === 'string' ? 'pipe ' + address : 'port ' + port;
	console.log('Listening on ' + bind);
});

server.listen(port);
