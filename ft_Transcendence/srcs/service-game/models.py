"""
models.py
SQLAlchemy Models — one class = one PostgreSQL table
"""

from sqlalchemy import Column, Integer, String, ForeignKey, DateTime, Boolean
from sqlalchemy.orm import relationship
from sqlalchemy.sql import func

from sqlalchemy.dialects.postgresql import UUID
import uuid

from database import Base

colours = ["Red", "Green", "Yellow", "Blue"]

CARD_POINTS = {
    "0": 0,
    "1": 1,
    "2": 2,
    "3": 3,
    "4": 4,
    "5": 5,
    "6": 6,
    "7": 7,
    "8": 8,
    "9": 9,
    "Skip": 20,
    "Reverse": 20,
    "Draw Two": 20,
    "Any": 50,
    "Draw Four": 50,
}

CARD_WEIGHTS = {
    "0": 0,

    "1": 1,
    "2": 1,
    "3": 1,
    "4": 1,
    "5": 1,

    "6": 2,
    "7": 2,
    "8": 2,
    "9": 2,

    "skip": 3,
    "reverse": 3,
    "draw2": 3,

    "wild": 5,
    "wild_draw4": 5,
}
class Card(Base):
    """
    Represents one card
    One table for all cards (draw, discard, hands)
    Location indicates where is the card
    """
    __tablename__ = "cards"

    id       = Column(Integer, primary_key=True)
    game_id  = Column(Integer, ForeignKey("games.id"), nullable=False)
    colour   = Column(String(20), nullable=False)
    value    = Column(String(20), nullable=False)
    location = Column(String(10), nullable=False)   # "deck", "discard", "hand"
    position = Column(Integer)                       # order in deck or discard
    owner_id = Column(Integer, ForeignKey("game_players.id"))  # NULL except if "hand"

    game     = relationship("Game", back_populates="cards")
    owner    = relationship("GamePlayer", back_populates="cards")

    def __repr__(self):
        return "{} {}".format(self.colour, self.value)


class Game(Base):
    __tablename__ = "games"

    id             = Column(Integer, primary_key=True)
    game_type      = Column(String(20), nullable=False)            # "uno"
    status         = Column(String(20), default="waiting")         # "waiting", "playing", "finished"
    nb_players     = Column(Integer, nullable=False)
    current_turn   = Column(Integer, default=0)
    direction      = Column(Integer, default=1)                    # 1 or -1
    current_colour = Column(String(20))
    current_value  = Column(String(20))
    created_at     = Column(DateTime, default=func.now())

    players        = relationship("GamePlayer", back_populates="game", order_by="GamePlayer.player_number")
    cards          = relationship("Card", back_populates="game")


class GamePlayer(Base):
    __tablename__ = "game_players"

    id            = Column(Integer, primary_key=True)
    ai            = Column(Boolean, default=False)
    user_id       = Column(UUID(as_uuid=True), nullable=True)
    game_id       = Column(Integer, ForeignKey("games.id"))
    player_number = Column(Integer, nullable=False)   # 0, 1, 2, 3
    result        = Column(String(10))                # "win", "loss", None if playing
    score         = Column(Integer, default=0)

    game          = relationship("Game", back_populates="players")
    cards         = relationship("Card", back_populates="owner")
