import https from "https";
import http from "http";
import fs from "fs";
import app from './app.js';
import { disconnectDB } from "./repository.js";

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

// charging secrets
const tlsOptions = {
	key: fs.readFileSync('/run/secrets/service_token_key'),
	cert: fs.readFileSync('/run/secrets/service_token_crt'),
	ca: fs.readFileSync('/run/secrets/ca_crt'),
	requestCert: true,
	rejectAuthorization: true,
}

const server = https.createServer(tlsOptions, app);
// const server = http.createServer(app);

// .on to listen to events
server.on('error', errorHandler);
server.on('listening', () => {
  const address = server.address();
  const bind = typeof address === 'string' ? 'pipe ' + address : 'port ' + port; //to know on which port (number or pipe) we are listening
  console.log('Listening on ' + bind);
});

server.listen(port);

const gracefulShutdown = async (signal) => {
	console.log(`\n${signal} received. Shutting down gracefully...`);
	
	// 1. Stop accepting new connections immediately
	server.close(async () => {
		console.log('HTTP server closed.');
		
		try {
			await disconnectDB();
						console.log('token-db connection closed.');

			console.log('Clean shutdown complete.');
			process.exit(0); // 0 = Success
		} catch (err) {
			console.error('Error during shutdown:', err);
			process.exit(1); // 1 = Exit with error
		}
	});

	// Fallback: If connections fail to close after 5 seconds, force exit
	setTimeout(() => {
		console.error('Forcefully terminating after 5 seconds of hanging.');
		process.exit(1);
	}, 5000);
};

// Listen for Podman/Docker's stop signal
process.on('SIGTERM', () => gracefulShutdown('SIGTERM'));

// Listen for Ctrl+C in your terminal
process.on('SIGINT', () => gracefulShutdown('SIGINT'));