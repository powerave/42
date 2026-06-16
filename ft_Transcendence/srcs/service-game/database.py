"""
database.py
Connexion to PostgreSQL form env variables defined in docker-compose.yml
"""
 
import os
from sqlalchemy import create_engine
from sqlalchemy.orm import DeclarativeBase, sessionmaker
 
#read DATABASE_ULR from the containers env variables
# Format : postgresql://USER:PASSWORD@HOST/DBNAME
# Exemple docker-compose : postgresql://admin:password@db/transcendence
DATABASE_URL = os.environ.get("DATABASE_URL")
 
if not DATABASE_URL:
    raise RuntimeError("DATABASE_URL is not defined in the environment.")
 
engine = create_engine(DATABASE_URL)
Session = sessionmaker(bind=engine)
 
class Base(DeclarativeBase):
    pass

def init_db():
    """Create all table if they don't exist"""
    from models import Game, GamePlayer, Card
    Base.metadata.create_all(bind=engine)
