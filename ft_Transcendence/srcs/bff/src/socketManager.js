import fs from "fs";
import { io as ioClient } from 'socket.io-client';
import { socketAuthMiddleware } from "./middlewares/socketAuthMiddleware.js"

import gameService from "./services/gameService.js";
import usersClient from "./clients/usersClient.js";
import gameClient from "./clients/gameClient.js";

import { redisClient } from './config/redis.js';


// ── Connexion vers service-game ──────────────────────────────────────────
export const gameSocket = ioClient('wss://service-game:8000', {
  ca:   fs.readFileSync('/run/secrets/ca_crt'),
  cert: fs.readFileSync('/run/secrets/bff_crt'),
  key:  fs.readFileSync('/run/secrets/bff_key'),
  // rejectUnauthorized: true, ??
});

export const connections = new Map();     // sid -> user_id
export const userToSid   = new Map();     // user_id -> sid
export const gameRooms   = new Map();     // game_id -> sid[]

export function initSocketManager(io) {

	io.use(socketAuthMiddleware);

	gameClient.setSocket(gameSocket);

	io.on('connection', (socket) => {
		// const userId = socket.handshake.auth?.user_id ?? null;
		console.log(`Client trying to connect`);
		const userId = socket.userUuid;
		console.log(`Client uuid : ${userId}`);
		connections.set(socket.id, userId);
		if (userId) userToSid.set(String(userId), socket.id);
		console.log(`Client connecté : ${socket.id}`);
		console.log(`Presence tracker connected for user: ${userId}`);
		
		redisClient.set(`presence:${userId}`, 'ONLINE', { EX: 86400 })

		socket.on('message', async (msg) => {
			// If JOIN_GAME -> save the player in the room room
			if (msg.type === 'JOIN_GAME') {
				const gameId = String(msg.game_id);
				//WARN : pas dangereux de creer la room si elle existe pas?
				//create devrait etre le seul a creer une room
				//comment gerer la room pleine?
				//Ne devrais pas attendre le retour de .emit?
				//du coup dupliquer l'envoie et mettre une condition sur celui d'en bas
				if (!gameRooms.has(gameId)) gameRooms.set(gameId, []);
				gameRooms.get(gameId).push(socket.id);
				//else send msg already in game
			}

			if (msg.type === 'RECONNECT') {
				const uid = String(userId);

				// Update ws map with the new client socket
				userToSid.set(uid, socket.id);

				console.log(`Presence tracker reconnected for user: ${userId}`);
		
				redisClient.set(`presence:${userId}`, 'ONLINE', { EX: 86400 })
				// Put player back in the room
				const gameId = String(msg.game_id);
				// WARN : pareil ici: ne pas creer la room si elle n'existe pas
				if (!gameRooms.has(gameId)) gameRooms.set(gameId, []);
				const room = gameRooms.get(gameId);
				if (!room.includes(socket.id)) room.push(socket.id);

				// Notify the game that the client came back
				await gameService.handleSocketMessage(
					{ type: 'PLAYER_RECONNECTED', game_id: msg.game_id, user_id: userId },
					gameSocket
				);
				return;
			}

			// delegate to gameService HTTPS + WSS if needed
			await gameService.handleSocketMessage({...msg, user_id: userId}, gameSocket)
		});

		socket.on('disconnect', () => {
			const uid = connections.get(socket.id);

			// WARN : Pas sur de laisser le timer
			// Do not remove socket from commections imediatly
			// player has 30 sec to reconnect
			setTimeout(() => {
				if (uid && userToSid.get(String(uid)) === socket.id) {
					userToSid.delete(String(uid));
				}
			}, 35_000);

			connections.delete(socket.id);

			console.log(`Presence tracker disconnected for user: ${userId}`);
			redisClient.del(`presence:${userId}`)

			// clean rooms
			// TODO : hum revoir ca parce que c pas clair c que ca fait
			for (const [gameId, sids] of gameRooms) {
				const filtered = sids.filter(s => s !== socket.id);
				//WARN : supprimer la room si elle est totalement vide?
				if (filtered.length === 0) gameRooms.delete(gameId);
				else gameRooms.set(gameId, filtered);
			}

			gameSocket.emit('message', { type: 'PLAYER_DISCONNECTED', user_id: userId });
		});
	});

	// service-game → BFF → front
	gameSocket.on('message', async (msg) => {
		const { target_user_id, game_id, ...payload } = msg;

		console.log('msg recu de service-game:', msg);
		console.log('userToSid:', userToSid);  // is the user in ?
		console.log('gameRooms:', gameRooms);  // ← is game_id 9 in ?

		// NOTE : ici faire appel a user pour remplacer (ou ajouter?) le pseudo dans message avant de l'envoyer au client
		const enriched = await gameService.enrichGameEvent(payload);

		// @AUDREY
		if (payload.type === "GAME_OVER")
			gameService.updateXPUser(game_id);

		// TODO : catch game update + hand update
		// 	batch user
		if (payload.type === "GAME_UPDATE" || payload.type === "HAND_UPDATE") {
			// console.log('user_ids raw:', payload.user_ids);
			// console.log('typeof:', typeof payload.user_ids[0], JSON.stringify(payload.user_ids));
			const userIds = (payload.user_ids ?? []).filter(Boolean);
			console.log('userIds filtered:', userIds);
			if (userIds.length) {
				const data = await usersClient.batchUserInfo(userIds);
				console.log('DATA', data.data);
				const usersInfo = data.data;

				if (payload.type === "GAME_UPDATE") {
            		payload.players = payload.players.map(player => {
                		const info = usersInfo[player.player_id];
                		return info
                    	? { ...player, username: info.username, xp: info.xp, avatar: info.avatar }
                    	: player;
					});
				}
				if (payload.type === "HAND_UPDATE") {
					const info = usersInfo[payload.target_user_id ?? userIds[0]];
					if (info) {
						payload.username = info.username;
						payload.xp = info.xp;
						payload.avatar = info.avatar;
					}
				}
			}
			console.log('Payload enriched with pseudo', payload);
		}

		if (target_user_id) {
			// Broadcast to one specific client
			const sid = userToSid.get(String(target_user_id));
			if (sid) io.to(sid).emit('message', {...enriched, game_id});
		} else if (game_id) {
			// Broadcast to the entire room
			const sids = gameRooms.get(String(game_id)) ?? [];
			for (const sid of sids) io.to(sid).emit('message', {...enriched, game_id});
		} else {
			// Broadcast to all clients
			io.emit('message', enriched);
		}
	});
}
