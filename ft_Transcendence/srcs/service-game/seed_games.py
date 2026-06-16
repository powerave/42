import os
from datetime import datetime, timedelta
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker

from database import Base
from models import Game, GamePlayer


def seed_game_statistics():
    DATABASE_URL = os.getenv("DATABASE_URL")
    engine = create_engine(DATABASE_URL)

    Base.metadata.create_all(engine)
    Session = sessionmaker(bind=engine)
    session = Session()

    print("Extracting test user uuids from environment variables...")

    p1 = os.getenv("T1_UUID")
    # p1 = 1
    p2 = os.getenv("T2_UUID")
    # p2 = 2
    p3 = os.getenv("T3_UUID")
    # p3 = 3
    p4 = os.getenv("T4_UUID")
    # p4 = 4

    #print(f"Loaded uuids -> P1: {p1}, P2: {p2}, P3: {p3}, P4: {p4}")
    print(f"Test int uuids -> P1: {p1}, P2: {p2}, P3: {p3}, P4: {p4}")

    try:
        if session.query(Game).first() is not None:
            print("Game data already present. Aborting seed.")
            return

        now = datetime.now()

        # Match 1:  (7 days ago)
        g1 = Game(game_type="uno", status="finished", nb_players=2, created_at=now - timedelta(days=7))
        g1.players.extend([
            GamePlayer(user_id=p1, player_number=0, result="loss", score=-10),
            GamePlayer(user_id=p2, player_number=1, result="win", score=2)
        ])

        # Match 2:  (5 days ago)
        g2 = Game(game_type="uno", status="finished", nb_players=2, created_at=now - timedelta(days=5))
        g2.players.extend([
            GamePlayer(user_id=p2, player_number=0, result="win", score=10),
            GamePlayer(user_id=p3, player_number=1, result="loss", score=-4)
        ])

        # Match 3: High-Score (3 days ago)
        g3 = Game(game_type="uno", status="finished", nb_players=3, created_at=now - timedelta(days=3))
        g3.players.extend([
            GamePlayer(user_id=p1, player_number=0, result="win", score=250), # Outlier for high-score stats
            GamePlayer(user_id=p2, player_number=1, result="loss", score=-45),
            GamePlayer(user_id=p3, player_number=2, result="loss", score=-12)
        ])

        # Match 4: Recent Finished UNO Match (1 day ago)
        g4 = Game(game_type="uno", status="finished", nb_players=3, created_at=now - timedelta(days=1))
        g4.players.extend([
            GamePlayer(user_id=p4, player_number=0, result="win", score=180),
            GamePlayer(user_id=p1, player_number=1, result="loss", score=-90),
            GamePlayer(user_id=p3, player_number=2, result="loss", score=-30)
        ])

        # Match 5: Active Live Game (to test exclusion of ongoing games)
        g5 = Game(game_type="uno", status="playing", nb_players=2, created_at=now)
        g5.players.extend([
            GamePlayer(user_id=p1, player_number=0, result=None, score=15),
            GamePlayer(user_id=p2, player_number=1, result=None, score=20)
        ])

        session.add_all([g1, g2, g3, g4, g5])
        session.commit()
        print("Game stats seeded successfully.")

    except Exception as e:
        session.rollback()
        print(f"Seeding gamestats failed: {e}")
        raise e
    finally:
        session.close()

if __name__ == "__main__":
    seed_game_statistics()
