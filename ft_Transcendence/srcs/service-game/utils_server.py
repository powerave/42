from models import Game
from utils_db import nb_players_in_game

def getList(active_games):
    """
    Return: All games and their number of player
        [
            {"game_id": "abc", "max_players": 4, "current_players": 2},
            {"game_id": "xyz", "max_players": 2, "current_players": 1}
        ]
    """
    games = []

    for game_id, game in active_games.items():
        current = nb_players_in_game(game_id)
        if current < game.state.nb_players:
            games.append({
                "game_id":          game_id,
                "max_players":      game.state.nb_players,
                "current_players":  current,
            })

    return games
