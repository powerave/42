"""
server.py

This files deals with HTTP requests and Websockets events
"""

import os
import uvicorn
import socketio
import ssl

import asyncio

from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel

from game   import Game
from models import colours
from utils_db import nb_players_in_game, get_user_ids, get_player_id, get_all_stats, get_player_stat, get_player_history_from_db, get_game_data_from_db, get_player
from utils import canPlay
from utils_server import getList

from database import init_db

from uuid import UUID

import random

# ─────────────────────────────────────────────────────────────────────────────
# Setup FastAPI + Socket.io
# ─────────────────────────────────────────────────────────────────────────────

sio = socketio.AsyncServer(
    async_mode      = 'asgi',
    cors_allowed_origins = '*'
)

app = FastAPI()
app.add_middleware(
    CORSMiddleware,
    allow_origins = ["*"],
    allow_methods = ["*"],
    allow_headers = ["*"],
)

# wrapping FastAPI with Socket.io so they share the same port
combined_app = socketio.ASGIApp(sio, other_asgi_app=app)

#Saving actives games here so that an object Game doesn't have to be made at each ws event
# { game_id: Game }
active_games = {}
disconnect_timers = {}   # { user_id: asyncio.TimerHandle }

def _remove_game(game_id: int):
    """Remove a game from active_games and close its DB session."""
    game = active_games.pop(game_id, None)
    if game:
        game.close()

# sid of the BFF (set on connect, used to emit back)
bff_sid = None


# ─────────────────────────────────────────────────────────────────────────────
# Pydantic Model — automatic validation of data received in HTTP
# ─────────────────────────────────────────────────────────────────────────────

class CreateGameRequest(BaseModel):
    num_players: int
    solo: bool


# ─────────────────────────────────────────────────────────────────────────────
# HTTPs Routes (FastAPI)
# ─────────────────────────────────────────────────────────────────────────────

@app.get("/api/game/health")
def health():
    """
    Check service health
    """
    return {"status": "ok"}


@app.post("/api/game/create")
async def create_game(body: CreateGameRequest):
    """
    Create a new game

    Return value: { game_id, nb_players, players, top_discard }
    """
    if not (2 <= body.num_players <= 4):
        raise HTTPException(status_code=400, detail="num_players must be between 2 and 4")

    # Create game via game.py
    game    = Game(body.num_players)
    game_id = game.state.id

    #create ai players
    if body.solo == 1:
        for i in range(game.state.nb_players - 1):
            game.createPlayer(user_id=None, ai=True)

    # Save game for next ws events
    active_games[game_id] = game

    state = game.getState()

    games = getList(active_games)
    await emit_to_bff({"type": "NEW_GAME", "games": games})

    return {
        "game_id":     game_id,
        "nb_players":  game.state.nb_players,
        "players":     state["players"],
        "top_discard": state["top_discard"],
    }

@app.get("/api/game/listGames")
def list_games():
    """
    Return: All games and their number of player
    """
    games = getList(active_games)

    return games

@app.get("/api/game/stats")
def get_stats():
    """Return all game stats:
        {"rank":1,"user_id":1,"wins":1,"losses":0,"total_games":1},
        {"rank":2,"user_id":2,"wins":0,"losses":1,"total_games":1}
    """

    return get_all_stats()

@app.get("/api/game/playerStats")
def get_player_stats(userId: UUID):
    """Return player game stats:
    {"user_uuid":"019f6fac-1110-76ab-85cf-2cc02d089182", games_cout:2, games_win:1, ratio:50}
    """

    return get_player_stat(userId)

@app.get("/api/game/playerHistory")
def get_player_history(userId: UUID):

    return get_player_history_from_db(userId)

@app.get("/api/game/gameData")
def get_game_data(gameId: int):
    return get_game_data_from_db(gameId)


# ─────────────────────────────────────────────────────────────────────────────
# WebSocket (Socket.io)
#
# Each @sio.event is a handler for a ws event.
# sid  = unique connexion id
# data = what the client sent
# ─────────────────────────────────────────────────────────────────────────────

@sio.event
async def connect(sid, environ, auth):
    """
    Called when the BFF opens a ws connexion.
    There is only one client: the BFF.
    """
    global bff_sid
    bff_sid = sid
    print(f"BFF connected : {sid}")


@sio.event
async def disconnect(sid):
    """
    Called when the BFF closes its ws connexion.
    """
    global bff_sid
    bff_sid = None
    print(f"BFF disconnected : {sid}")

import uuid

def serialize_state(obj):
    if isinstance(obj, dict):
        return {k: serialize_state(v) for k, v in obj.items()}
    elif isinstance(obj, list):
        return [serialize_state(i) for i in obj]
    elif isinstance(obj, uuid.UUID):
        return str(obj)
    return obj

async def emit_to_bff(msg: dict):
    """Helper to send a message back to the BFF."""
    if bff_sid:
        msg = serialize_state(msg)
        await sio.emit("message", msg, to=bff_sid)


@sio.event
async def message(sid, msg):
    handlers = {
        "JOIN_GAME":           handle_join_game,
        "PLAY_CARD":           handle_play_card,
        "DRAW_CARD":           handle_draw_card,
        "CHOOSE_COLOUR":       handle_choose_colour,
        "PLAYER_DISCONNECTED": handle_player_disconnected,
        "PLAYER_RECONNECTED":  handle_player_reconnected,
        "LEAVE_GAME":          handle_leave_game,
    }
    msg_type = msg.get("type")
    handler  = handlers.get(msg_type)
    if not handler:
        await emit_to_bff({"type": "ERROR", "message": f"Unknown message type: {msg_type}"})
        return
    print(f"Message received from BFF (type: {msg_type}) : {msg}")
    await handler(sid, msg)


async def handle_join_game(sid, msg):
    """
    msg = { "type": "JOIN_GAME", "game_id": 7, "user_id": 42 }
    """
    game_id = msg.get("game_id")
    user_id = msg.get("user_id")

    game = active_games.get(game_id)
    if not game:
        await emit_to_bff({"type": "ERROR", "target_user_id": user_id, "message": "Game not found"})
        return

    user_ids = get_user_ids(game_id)
    for uid in user_ids:
        if str(uid) == user_id:
            await emit_to_bff({"type": "ERROR", "target_user_id": user_id, "message": "Already in game"})
            return

    if game.state.nb_players == nb_players_in_game(game_id):
        await emit_to_bff({"type": "ERROR", "target_user_id": user_id, "message": "Game full"})
        return

    player_id = game.createPlayer(user_id=user_id, ai=False)
    print(f"User {user_id} joined game {game_id} as player {player_id}")

    # Send initial state to this player only
    state = game.getState()
    await emit_to_bff({"type": "GAME_UPDATE", "target_user_id": user_id, **state, "user_ids": [user_id]})

    games = getList(active_games)
    await emit_to_bff({"type": "NEW_GAME", "games": games})


    # When room is full: start the game and send hands to everyone
    if game.state.nb_players == nb_players_in_game(game_id):
        game.initGame()
        state = game.getState()
        user_ids = get_user_ids(game_id)
        # Broadcast state to all players in the game
        await emit_to_bff({"type": "GAME_UPDATE", "game_id": game_id, "user_ids": user_ids, **state})

        # Send each player their hand
        for uid in user_ids:
            player = get_player(game_id, uid)
            if not player.ai:
                hand   = game.getHand(player.id)
                print(f"Sending hand to user {uid} in game {game_id}")
                await emit_to_bff({
                    "type":           "HAND_UPDATE",
                    "target_user_id": uid,
                    "hand":           hand,
                    "player_id":      player.id,
                    "player_number": player.player_number,
                    "game_id":        game_id,
                    "user_ids": [uid],
                })

    await play_bot(game_id, game)


async def handle_play_card(sid, msg):
    """
    msg = { "type": "PLAY_CARD", "game_id": 7, "user_id": 42, "player_id": 3, "card_index": 2 }
    """
    game_id    = msg.get("game_id")
    user_id    = msg.get("user_id")
    player_id  = msg.get("player_id")
    card_index = msg.get("card_index")

    game = active_games.get(game_id)
    if not game:
        await emit_to_bff({"type": "ERROR", "target_user_id": user_id, "message": "Game not found"})
        return

    result = game.playCard(player_id, card_index)

    if not result["success"]:
        await emit_to_bff({"type": "ERROR", "target_user_id": user_id, "message": result["error"]})
        return

    # ── Send updates to all players ──────────────────────────────────────
    state = game.getState()
    user_ids = get_user_ids(game_id)

    await emit_to_bff({"type": "GAME_UPDATE", "game_id": game_id, "user_ids": user_ids, **state})

    for uid in user_ids:
        player = get_player(game_id, uid)
        if not player.ai:
            hand   = game.getHand(player.id)
            await emit_to_bff({
                "type":           "HAND_UPDATE",
                "target_user_id": uid,
                "hand":           hand,
                "player_id":      player.id,
                "effect":         result.get("special_effect"),
                "drew_player":    result.get("drew_player"),
                "drew_count":     result.get("drew_count"),
                "user_ids":       [uid],
            })

    # ── Game finished ? ──────────────────────────────────────────────────
    if result["game_over"]:
        await emit_to_bff({"type": "GAME_OVER",   "game_id": game_id, "winner_player_number": result["winner"]})
        _remove_game(game_id)
        return

    # ── Wild : Ask the colour to the player ─────────────────────────────
    if result.get("needs_colour"):
        await emit_to_bff({"type": "CHOOSE_COLOUR",  "target_user_id": user_id, "player_id": player_id})
        return

    await play_bot(game_id, game)



async def handle_choose_colour(sid, msg):
    """
    msg = { "type": "CHOOSE_COLOUR", "game_id": 7, "user_id": 42, "player_id": 3, "colour": "Red" }
    """
    game_id   = msg.get("game_id")
    user_id   = msg.get("user_id")
    player_id = msg.get("player_id")
    colour    = msg.get("colour")

    game = active_games.get(game_id)
    if not game:
        await emit_to_bff({"type": "ERROR", "target_user_id": user_id, "message": "Game not found"})
        return

    result = game.setWildColour(player_id, colour)

    if not result["success"]:
        await emit_to_bff({"type": "ERROR", "target_user_id": user_id, "message": result["error"]})
        return

    state = game.getState()
    user_ids = get_user_ids(game_id)

    await emit_to_bff({"type": "GAME_UPDATE", "game_id": game_id, "user_ids": user_ids, **state})

    await play_bot(game_id, game)


async def handle_draw_card(sid, msg):
    """
    msg = { "type": "DRAW_CARD", "game_id": 7, "user_id": 42, "player_id": 3 }
    """
    game_id   = msg.get("game_id")
    user_id   = msg.get("user_id")
    player_id = msg.get("player_id")

    game = active_games.get(game_id)
    if not game:
        await emit_to_bff({"type": "ERROR", "target_user_id": user_id, "message": "Game not found"})
        return

    result = game.drawCard(player_id)

    if not result["success"]:
        await emit_to_bff({"type": "ERROR", "target_user_id": user_id, "message": result["error"]})
        return

    state = game.getState()
    hand  = game.getHand(player_id)
    user_ids = get_user_ids(game_id)

    await emit_to_bff({"type": "GAME_UPDATE", "game_id": game_id, "user_ids": user_ids, **state})
    await emit_to_bff({"type": "HAND_UPDATE", "target_user_id": user_id, "hand": hand, "player_id": player_id, "user_ids": [user_id], "game_id": game_id})

    await play_bot(game_id, game)


async def play_bot(game_id: int, game: Game):
    """Play bot turn"""
    if game.state.nb_players != nb_players_in_game(game_id):
        print(f"Game {game_id} not full yet, bot won't play")
        return

    user_ids = get_user_ids(game_id)
    human_players = [
        uid for uid in user_ids 
        if not get_player(game_id, uid).ai
    ]
    if not human_players:
        print(f"Game {game_id} without humans — automatic shutdown.")
        _remove_game(game_id)
        return

    while True:

        # Refresh game state from DB
        game.db.expire_all()
        game.db.refresh(game.state)

        current = game.getCurrentPlayer()
        if not current.ai:
            break

        await asyncio.sleep(0.8)

        hand = game._getHand(current.id)
        state = game.getState()
        topDiscard = game.getTopDiscard()
        current_colour = state.get("current_colour", topDiscard["colour"])
        current_value = state.get("current_value", topDiscard["value"])
        result = {"game_over":False}

        print(f"\033[94m--- [BOT ACTION] ---\033[0m")
        print(f"Bot's turn (Number: {current.player_number} | ID: {current.id})")
        print(f"Bot see on the table : Colour={state.get('current_colour')}, Value={state.get('current_value')}")
        print(f"Bot see his hand : {[(c.colour, c.value) for c in hand]}")
        print(f"Bot thinks he can play : {canPlay(state.get('current_colour'), state.get('current_value'), hand)}")
        print(f"--------------------")

        if current_colour == "Wild":
            result = game.setWildColour(current.id, random.choice(colours))
            continue

        if not canPlay(current_colour, current_value, hand):
            result = game.drawCard(current.id)

        else:
            for card_index, card in enumerate(hand):
                if canPlay(current_colour, current_value, [card]):
                    result = game.playCard(current.id, card_index)

                    if result.get("needs_colour"):
                        result = game.setWildColour(current.id, random.choice(colours))

                    elif "success" in result and not result["success"]:
                        print(f"⚠️ BOT DID AN INVALID PLAY ! Erreur: {result.get('error')}")

                    elif result["game_over"]:
                        state = game.getState()
                        user_ids = get_user_ids(game_id)

                        await emit_to_bff({"type": "GAME_UPDATE", "game_id": game_id, "user_ids": user_ids, **state})
                        await emit_to_bff({"type": "GAME_OVER",   "game_id": game_id, "winner_player_number": result["winner"]})
                        _remove_game(game_id)
                        return

                    break

        state = game.getState()
        user_ids = get_user_ids(game_id)

        await emit_to_bff({"type": "GAME_UPDATE", "game_id": game_id, "user_ids": user_ids, **state})

        for uid in user_ids:
            player = get_player(game_id, uid)
            if not player.ai:
                hand   = game.getHand(player.id)
                await emit_to_bff({
                    "type":           "HAND_UPDATE",
                    "target_user_id": uid,
                    "hand":           hand,
                    "player_id":      player.id,
                    "effect":         result.get("special_effect"),
                    "drew_player":    result.get("drew_player"),
                    "drew_count":     result.get("drew_count"),
                    "user_ids":       [uid],
                })


async def handle_player_disconnected(sid, msg):
    """
    msg = { "type": "PLAYER_DISCONNECTED", "user_id": 42 }
    Start a timer of 30 sec. If player does not come back replace by a bot
    """
    user_id = msg.get("user_id")
    print(f"Player {user_id} disconnected — timer 30s started")

    from utils_db import get_user_ids

    print(type(user_id))
    
    game_id = None
    for gid, game in active_games.items():
        print(type(get_user_ids(gid)[0]))
        if uuid.UUID(user_id) in get_user_ids(gid):
            game_id = gid
            break

    if game_id is None:
        print(f"No game found to replace disconnected player {user_id}")
        return

    existing = disconnect_timers.pop(user_id, None)
    if existing:
        existing.cancel()

    loop = asyncio.get_event_loop()
    handle = loop.call_later(
        30,
        lambda: asyncio.ensure_future(_replace_with_bot(user_id, game_id))
    )
    disconnect_timers[user_id] = handle



async def _replace_with_bot(user_id: int, game_id: int):
    """Replace player by a bot."""
    disconnect_timers.pop(user_id, None)

    game = active_games.get(game_id)
    if not game:
        return

    player_id = get_player_id(game_id, user_id)
    if not player_id:
        return

    if game.state.status == "finished":
        print(f"Game {game_id} already finished — skipping bot replacement for {user_id}")
        return

    print(f"Player {user_id} disconnected — replaced by bot in game {game_id}")
    game.replacePlayerWithBot(player_id)

    await emit_to_bff({
        "type":    "PLAYER_REPLACED_BY_BOT",
        "game_id": game_id,
        "user_id": user_id,
    })

    await play_bot(game_id, game)


async def handle_player_reconnected(sid, msg):
    """
    msg = { "type": "PLAYER_RECONNECTED", "user_id": 42, "game_id": 7 }
    """
    game_id = msg.get("game_id")
    user_id = msg.get("user_id")

    timer = disconnect_timers.pop(user_id, None)
    if timer:
        timer.cancel()
        print(f"Timer annuled for user {user_id} — reconnexion in time")

    game = active_games.get(game_id)
    if not game:
        await emit_to_bff({"type": "RECONNECT_DENIED", "target_user_id": user_id})
        return

    player_id = get_player_id(game_id, user_id)
    if not player_id:
        await emit_to_bff({"type": "RECONNECT_DENIED", "target_user_id": user_id})
        return

    state = game.getState()
    hand  = game.getHand(player_id)

    await emit_to_bff({"type": "GAME_UPDATE",  "target_user_id": user_id, "user_ids": [user_id], **state})
    await emit_to_bff({"type": "HAND_UPDATE",  "target_user_id": user_id, "hand": hand, "player_id": player_id, "user_ids": [user_id], "game_id": game_id})


async def handle_leave_game(sid, msg):
    """
    msg = { "type": "LEAVE_GAME", "game_id": 7, "user_id": 42 }
    Player leave volontarely — annul timer if there is one.
    """
    user_id = msg.get("user_id")
    game_id = msg.get("game_id")

    # annul timer if exists (ex: leaving while in grace period after disconnection)
    timer = disconnect_timers.pop(user_id, None)
    if timer:
        timer.cancel()

    print(f"User {user_id} left voluntarily {game_id}")

    await _replace_with_bot(user_id, game_id)

# ─────────────────────────────────────────────────────────────────────────────
# Starting server
# ─────────────────────────────────────────────────────────────────────────────

if __name__ == "__main__":
    init_db()
    PORT = int(os.environ.get("PORT", 8000))
    uvicorn.run(combined_app, host="0.0.0.0", port=PORT,
        ssl_keyfile="/run/secrets/service_game_key",
        ssl_certfile="/run/secrets/service_game_crt",
        ssl_ca_certs="/run/secrets/ca_crt",
        ssl_cert_reqs=ssl.CERT_REQUIRED,
    )
