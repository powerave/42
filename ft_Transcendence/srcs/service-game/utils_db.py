from database import Session
from models   import Game, GamePlayer
from sqlalchemy import func, case

def nb_players_in_game(game_id):
    with Session() as session:
        game = session.query(Game).get(game_id)
        return len(game.players)

def get_user_ids(game_id: int):
    with Session() as session:
        return [
            gp.user_id
            for gp in session.query(GamePlayer)
            .filter(GamePlayer.game_id == game_id)
            .all()
        ]

def get_player_id(game_id: int, user_id: int):
    with Session() as session:
        game_player = (session.query(GamePlayer)
        .filter(
            (GamePlayer.game_id == game_id) &
            (GamePlayer.user_id == user_id))
            .first())
        return game_player.id if game_player else None

def get_player(game_id: int, user_id: int):
    with Session() as session:
        game_player = (session.query(GamePlayer)
        .filter(
            (GamePlayer.game_id == game_id) &
            (GamePlayer.user_id == user_id))
            .first())
        return game_player if game_player else None

def get_all_stats():
    with Session() as session:
        rows = (
            session.query(
                GamePlayer.user_id,
                func.count(
                    case((GamePlayer.result == "win", 1))
                ).label("wins"),
                func.count(
                    case((GamePlayer.result == "loss", 1))
                ).label("losses"),
                func.count(GamePlayer.id).label("total_games"),
            )
            .join(Game, Game.id == GamePlayer.game_id)
            .filter(Game.status == "finished")
            .filter(GamePlayer.user_id.isnot(None))
            .group_by(GamePlayer.user_id)
            .order_by(
                func.count(case((GamePlayer.result == "win", 1))).desc(),
                func.count(case((GamePlayer.result == "loss", 1))).asc(),
            )
            .limit(10)
            .all()
        )
 
    leaderboard = []
    for rank, row in enumerate(rows, start=1):
        leaderboard.append({
            "rank":        rank,
            "user_id":     str(row.user_id),
            "wins":        row.wins,
            "losses":      row.losses,
            "total_games": row.total_games,
        })
 
    return leaderboard

def get_player_stat(user_id):
    with Session() as session:
        stats = (
            session.query(
                func.count(GamePlayer.game_id).label("games_count"),
                func.sum(
                    case(
                        (GamePlayer.result == "win", 1),
                        else_=0
                    )
                ).label("win_count")
            )
            .join(Game, Game.id == GamePlayer.game_id)
            .filter(GamePlayer.user_id == user_id)
            .filter(Game.status == "finished")
            .one()
        )

    games_count = stats.games_count or 0
    win_count = stats.win_count or 0

    return {
        "user_uuid": user_id,
        "games_count": games_count,
        "win_count": win_count,
        "ratio": round((win_count / games_count) * 100, 2) if games_count else 0,
    }

def get_player_history_from_db(user_id):
    with Session() as session:
        # 1. Get games were this user_id played
        played_game_ids = (
            session.query(GamePlayer.game_id)
            .filter(GamePlayer.user_id == user_id)
            .subquery()
        )
     
        # 2. Get finished games corresponding
        games = (
            session.query(Game)
            .filter(Game.id.in_(played_game_ids))
            .filter(Game.status == "finished")
            .order_by(Game.created_at.desc())
            .all()
        )
 
        if not games:
            return []
     
        # 3. Build response
        history = []
        for game in games:
            # Résults of the asked player
            my_entry = next(
                (p for p in game.players if p.user_id == user_id), None
            )
            my_result = my_entry.result if my_entry else None
     
            players_list = [
                {
                    "user_id": p.user_id,
                    "result":  p.result,
                    "score":   p.score,
                }
                for p in game.players
                if p.user_id is not None
            ]
     
            history.append({
                "game_id":    game.id,
                "game_type":  game.game_type,
                "created_at": game.created_at.isoformat() if game.created_at else None,
                "my_result":  my_result,   # "win" | "loss" | None
                "players":    players_list,
            })
 
    return history

def get_game_data_from_db(gameId):
    with Session() as session:
            players = (
                session.query(GamePlayer.user_id, GamePlayer.score)
                .filter(GamePlayer.game_id == gameId)
                .filter(GamePlayer.user_id.isnot(None))
                .all()
            )

            return {
                "game_id": gameId,
                "players": [
                    {
                        "user_id": user_id,
                        "score": score,
                    }
                    for user_id, score in players
                ]
            }
