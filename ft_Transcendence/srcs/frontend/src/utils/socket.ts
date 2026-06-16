import type { Socket } from 'socket.io-client';
import { ref } from 'vue';

declare const io: any;

let socketInstance: Socket | null = null;
// let userSocketInstance: Socket | null = null;

const subscribers: Map<string, Set<Function>> = new Map();
let listenersRegistered = false;


function notify(event: string, data: any) { // Notify all subscribers of an event
  const set = subscribers.get(event);
  if (!set) return;
  set.forEach((cb) => {
    try { cb(data); } catch (e) { /*console.error('socket subscriber error', e); */ null }
  });
}

function ensureListeners(socket: Socket) { // Ensure listeners are registered only once
  if (listenersRegistered) return;

  socket.on('connect', () => notify('connect', { id: socket.id }));
  socket.on('disconnect', (reason: any) => notify('disconnect', reason));
  socket.on('connect_error', (err: any) => notify('connect_error', err));

  // Canal unique pour BFF/Service-Game
  socket.on('message', (data: any) => notify('message', data));

  listenersRegistered = true;
}

export function initSocket(userId?: number | string, token?: string | null) { // Initialize socket connection with optional user ID for authentication
  if (socketInstance) return socketInstance;

  socketInstance = io('/', {
    transports: ['websocket'],
    withCredentials: true,
    auth: { 
      user_id: userId,
      token: token
    }
  });

  ensureListeners(socketInstance!); // save the instance before registering listeners
  return socketInstance; 
}

export function getSocket() { // Get the initialized socket instance
  return socketInstance;
}

// export function initUserSocket(userId?: number | string, token?: string | null) {
//   if (userSocketInstance) return userSocketInstance;
  
//   userSocketInstance = io('/user', {
//     transports: ['websocket'],
//     auth: {
//       user_id: userId,
//       token: token
//     }
//   });

//   userSocketInstance.on('user_status_changed', (data: { user_uuid: string, status: string }) => {
//     notify('user_status_changed', data);
//   });
//   userSocketInstance.on('connect_error', (err: any) => {
//     console.error('User Socket Error:', err);
//   });

//   return userSocketInstance;
// }

// export function disconnectUserSocket() {
//   if (userSocketInstance) {
//     userSocketInstance.disconnect();
//     userSocketInstance = null;
//   }
// }

export function subscribe(event: string, cb: Function) { // Subscribe to a socket event
  if (!subscribers.has(event)) subscribers.set(event, new Set());
  const set = subscribers.get(event)!;
  set.add(cb);
  return () => set.delete(cb);
}

export function clearAllSubscribers() {
  subscribers.clear();
}

export function disconnectSocket() {
  if (socketInstance) {
    socketInstance.disconnect();
    socketInstance = null;
    listenersRegistered = false;
  }
}
