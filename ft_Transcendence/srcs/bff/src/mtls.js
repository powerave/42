import https from 'https'
// import WebSocket from 'ws'
import fs from 'fs'

// Chargement une seule fois au démarrage
export const mtlsAgent = new https.Agent({
  cert: fs.readFileSync('/run/secrets/bff_crt'),   // le BFF se présente
  key:  fs.readFileSync('/run/secrets/bff_key'),
  ca:   fs.readFileSync('/run/secrets/ca_crt'),         // vérifie le serveur
  rejectUnauthorized: true,                        // OBLIGATOIRE
})

export const mtlsOptions = {
  cert: fs.readFileSync('/run/secrets/bff_crt'),
  key:  fs.readFileSync('/run/secrets/bff_key'),
  ca:   fs.readFileSync('/run/secrets/ca_crt'),
  rejectUnauthorized: true,
}

// export const pythonWs = new WebSocket('wss://service-game:8000/ws', mtlsOptions)

// export default {
// 	mtlsAgent,
// 	mtlsOptions,
// };


/*
// Appel HTTP vers un microservice en mTLS
export async function callService(serviceName, path, options = {}) {
  const url = `https://${serviceName}:4000${path}`
  const res = await fetch(url, {
    ...options,
    agent: mtlsAgent,  // Node 18+ accepte agent dans fetch via undici
  })
  if (!res.ok) throw new Error(`${serviceName} returned ${res.status}`)
  return res.json()
}*/

// /*
export function forwardToPythonWS(clientWs, message) {
  const pythonWs = new WebSocket('wss://service-python:5000/ws', mtlsOptions)

  pythonWs.on('open', () => {
    pythonWs.send(message)
  })

  pythonWs.on('message', (data) => {
    if (clientWs.readyState === WebSocket.OPEN) {
      clientWs.send(data)
    }
  })

  pythonWs.on('error', (err) => {
    console.error('Erreur WS Python:', err.message)
  })
}
// */

// export async function getGameHealth() {
//   return new Promise((resolve, reject) => {
//     const req = https.request(
//       'https://service-game:8000/api/game/health',
//       { agent: mtlsAgent },
//       (res) => {
//         let data = ''
//         res.on('data', chunk => data += chunk)
//         res.on('end', () => resolve(JSON.parse(data)))
//       }
//     )
//     req.on('error', reject)
//     req.end()
//   })
// }

// export async function getGameHealth() {
//   const url = `https://service-game:8000/api/game/health`
//   const res = await fetch(url, { agent: mtlsAgent })
//   if (!res.ok) throw new Error(`Service-game returned ${res.status}`)
//   return res.json()
// }
