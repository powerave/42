"""
game.py
Game logic, no web interaction here
"""

from database import Session
from models   import Game as GameModel, Card as CardModel, GamePlayer, colours, CARD_POINTS, CARD_WEIGHTS
from utils    import buildDeck, shuffleDeck, canPlay, setNextPlayer, updateCurrentDiscard
from utils_db import nb_players_in_game

import random


class Game:

    # ──────────────────────────────────────────────────────────────────────
    # INIT — Create a new game
    # ──────────────────────────────────────────────────────────────────────

    def __init__(self, numPlayers):
        """
        Create new Game in db + create and shuffle deck

        Parameters: numPlayers -> int (2 to 4)
        """
        self.db = Session()

        # ── Create game in DB ──────────────────────────────────────────
        self.state = GameModel(
            game_type  = "uno",
            status     = "playing",
            nb_players = numPlayers,
        )
        self.db.add(self.state)
        self.db.commit()    # needed to have self.state.id


        # ── Build and shuffle the deck ─────────────────────────────────
        deck = shuffleDeck(shuffleDeck(buildDeck()))
        for i, card in enumerate(deck):
            card.game_id  = self.state.id
            card.position = i
            self.db.add(card)
        self.db.commit()

    def createPlayer(self, user_id, ai):
        """
        Create a player in GamePlayer table
        Arguments: class instance, user_id associated with player
        Return: player_id
        """
        nb_player = nb_players_in_game(self.state.id)
        player = GamePlayer(
            user_id         = user_id,
            game_id         = self.state.id,
            player_number   = nb_player,
            ai              = ai,
        )
        self.db.add(player)
        self.db.commit()

        # Refresh players in db to get their ids
        self.db.refresh(self.state)

        return nb_player

    def initGame(self):
        """
        Give each player their hand
        Pick first discard card
        """
        # Pick players cards
        for player in self.state.players:
            hand = self._pickFromDeck(5)
            for card in hand:
                card.location = "hand"
                card.owner_id = player.id
        self.db.commit()

        # Pick first discard card
        first_card = self._pickFromDeck(1)[0]
        while first_card.colour == "Wild":
            first_card.position = self._maxDeckPosition() + 1
            self.db.commit()
            first_card = self._pickFromDeck(1)[0]

        first_card.location = "discard"
        first_card.position = 0
        self.state.current_colour = first_card.colour
        self.state.current_value  = first_card.value
        self.db.commit()

    # ──────────────────────────────────────────────────────────────────────
    # HELPERS DB — SQL requests encapsulated
    # ──────────────────────────────────────────────────────────────────────

    def _pickFromDeck(self, n):
        """
        Draw n first cards
        Doesn't change their location, its the caller's job
        """
        return (
            self.db.query(CardModel)
            .filter(
                CardModel.game_id  == self.state.id,
                CardModel.location == "deck"
            )
            .order_by(CardModel.position)
            .limit(n)
            .all()
        )

    def _getHand(self, player_id):
        """
        Return: player hand
        """
        return (
            self.db.query(CardModel)
            .filter(
                CardModel.game_id  == self.state.id,
                CardModel.location == "hand",
                CardModel.owner_id == player_id
            )
            .all()
        )

    def _getTopDiscard(self):
        """
        Return: top discard card
        """
        return (
            self.db.query(CardModel)
            .filter(
                CardModel.game_id  == self.state.id,
                CardModel.location == "discard"
            )
            .order_by(CardModel.position.desc())
            .first()
        )

    def _maxDeckPosition(self):
        """
        Return: Max Card position in deck (to put a card at the end of the deck)
        """
        card = (
            self.db.query(CardModel)
            .filter(
                CardModel.game_id  == self.state.id,
                CardModel.location == "deck"
            )
            .order_by(CardModel.position.desc())
            .first()
        )
        return card.position if card else 0

    def _maxDiscardPosition(self):
        """
        Return: Max Card position in discards pile (to pile played card)
        """
        top = self._getTopDiscard()
        return top.position if top else 0

    def _reshuffleDiscardIntoDeck(self):
        """
            Reshuffle the discard pile into the deck when the deck is empty.
        """

        # Keep the top card of the discard pile
        top = self._getTopDiscard()

        # Recover the rest of the discard pile (except the top card)
        old_discard = (
            self.db.query(CardModel)
            .filter(
                CardModel.game_id  == self.state.id,
                CardModel.location == "discard",
                CardModel.id       != top.id
            )
            .all()
        )

        if not old_discard:
            return False

        # Put back the cards in the deck and shuffle them
        random.shuffle(old_discard)
        for i, card in enumerate(old_discard):
            card.location = "deck"
            card.owner_id = None
            card.position = i

        self.db.commit()
        print(f"\033[96m[RESHUFFLE] Deck re shuffled ({len(old_discard)} cards)\033[0m")
        return True

    # ──────────────────────────────────────────────────────────────────────
    # NAVIGATION — Change player
    # ──────────────────────────────────────────────────────────────────────

    def advanceToNextPlayer(self):
        """
        Go to next player and save in DB
        """
        self.state.current_turn = setNextPlayer(
            self.state.current_turn,
            self.state.nb_players,
            self.state.direction
        )
        self.db.commit()

    # ──────────────────────────────────────────────────────────────────────
    # ACTIONS — What th eplayers can do
    # They always return a dict
    # ──────────────────────────────────────────────────────────────────────

    def playCard(self, player_id, card_index):
        """
        Player playing the card at card_index in his hand

        Parameters:
            player_id  -> int : player id (GamePlayer.id)
            card_index -> int : 0-based index in player's hand

        Return value: dict with following keys :
            "success"       -> bool   : False if error
            "error"         -> str    : error message if success=False
            "played_card"   -> str    : played card ex: "Red 7"
            "game_over"     -> bool   : True if this player won
            "winner"        -> int    : player_number of the winner if game_over
            "needs_colour"  -> bool   : True if Wild, player need to choose
            "special_effect"-> str    : "reverse", "skip", "draw_two", "draw_four" or None
            "drew_player"   -> int    : player_number of who had to draw (Draw Two/Four)
            "drew_count"    -> int    : nuber of cards drawn (2 or 4)
        """
        current_player = self.state.players[self.state.current_turn]

        # ── checks ─────────────────────────────────────────────────

        # Verify it is the player's turn
        if current_player.id != player_id:
            return {"success": False, "error": "Not your turn"}

        hand = self._getHand(player_id)

        # Verify that the index is valid
        if card_index < 0 or card_index >= len(hand):
            return {"success": False, "error": "Invalid card index"}

        played_card = hand[card_index]

        print(f"--- [MOTEUR : playCard] ---")
        print(f"Player Number: \033[94m{current_player.player_number}\033[0m (ID: {player_id}) | is it his turn ? {self.state.players[self.state.current_turn].id == player_id}")
        print(f"Index asked : {card_index} -> Cards in BD : {played_card.colour} {played_card.value}")
        print(f"Game state : Active coulour = {self.state.current_colour} | Active value = {self.state.current_value}")
        print(f"Result validation canPlay : {canPlay(self.state.current_colour, self.state.current_value, [played_card])}")
        print(f"---------------------------")


        # Verify that the card is playable
        if not canPlay(self.state.current_colour, self.state.current_value, [played_card]):
            return {"success": False, "error": "This card is not playable"}

        # Play card
        played_card.location = "discard"
        played_card.owner_id = None
        played_card.position = self._maxDiscardPosition() + 1
        self.state.current_colour, self.state.current_value = updateCurrentDiscard(played_card)
        self.db.commit()

        # Verify victory
        if len(self._getHand(player_id)) == 0:
            self.state.status     = "finished"
            current_player.result = "win"
            for p in self.state.players:
                if p.id != player_id:
                    p.result = "loss"
            self.db.commit()
            self.setScores()

            print(f"\033[92m")
            print(f"==================================================")
            print(f"🎉🏆 END OF GAME ! PLAYER number: {current_player.player_number} WON ! 🏆🎉")
            print(f"Last card played : {played_card}")
            print(f"--------------------------------------------------")
            print(f"Final score tab :")
            
            for p in self.state.players:
                status_icon = "👑 WINNER" if p.id == player_id else "❌ LOSER"
                print(f"  - Player number: \033[94m{p.player_number}\033[92m | {status_icon} | Score : {p.score} pts | Cards left : {len(self._getHand(p.id))}")
                
            print(f"==================================================\033[0m")

            return {
                "success":     True,
                "played_card": str(played_card),
                "game_over":   True,
                "winner":      current_player.player_number,
            }

        # Special card effect

        result = {
            "success":        True,
            "played_card":    str(played_card),
            "game_over":      False,
            "needs_colour":   False,
            "special_effect": None,
            "drew_player":    None,
            "drew_count":     None,
        }

        if self.state.current_value == "Reverse":
            self.state.direction *= -1
            if self.state.nb_players == 2:
                self.advanceToNextPlayer()
            self.db.commit()
            sens = "Reverse (Conterclockwise ↩️)" if self.state.direction == -1 else "Normal (Clockwise ↪️)"
            print(f"\033[35m🔄 [REVERSE] Game sens change ! New sens : {sens}. Next player: \033[94m{self.state.players[self.state.current_turn].player_number + self.state.direction}\033[35m\033[0m")
            result["special_effect"] = "reverse"

        elif self.state.current_value == "Skip":
            # Skip next player
            self.advanceToNextPlayer()
            skipped_player = self.state.players[self.state.current_turn]
            print(f"⏩ [SKIP] Player number : \033[94m{skipped_player.player_number}\033[0m (ID: {skipped_player.id}) turns was skipped.")
            result["special_effect"] = "skip"

        elif self.state.current_value == "Draw Two":
            # Calculates who is the next player BEFORE advancing
            next_turn   = setNextPlayer(
                self.state.current_turn,
                self.state.nb_players,
                self.state.direction
            )
            next_player = self.state.players[next_turn]
            if len(self._pickFromDeck(2)) < 2:
                self._reshuffleDiscardIntoDeck()
            drawn = self._pickFromDeck(2)
            print(f"\033[33m[FORCED DRAW] Player number: {next_player.player_number} (ID: {next_player.id}) draw {len(drawn)} cards because of an effect.\033[0m")
            for card in drawn:
                card.location = "hand"
                card.owner_id = next_player.id
            self.db.commit()
            result["special_effect"] = "draw_two"
            result["drew_player"]    = next_player.player_number
            result["drew_count"]     = 2
            self.advanceToNextPlayer()

        elif self.state.current_value == "Draw Four":
            next_turn   = setNextPlayer(
                self.state.current_turn,
                self.state.nb_players,
                self.state.direction
            )
            next_player = self.state.players[next_turn]
            if len(self._pickFromDeck(4)) < 4:
                self._reshuffleDiscardIntoDeck()
            drawn = self._pickFromDeck(4)
            print(f"\033[33m[FORCED DRAW] Player number: {next_player.player_number} (ID: {next_player.id}) draw {len(drawn)} cards because of an effect.\033[0m")
            for card in drawn:
                card.location = "hand"
                card.owner_id = next_player.id
            self.db.commit()
            result["special_effect"] = "draw_four"
            result["drew_player"]    = next_player.player_number
            result["drew_count"]     = 4

        # Wild : return needs_colour=True.
        # server.py will ask the player the color then call setWildColour()
        if played_card.colour == "Wild":
            return {
                "success":      True,
                "played_card":  str(played_card),
                "game_over":    False,
                "needs_colour": True,
                "special_effect": None,
            }

        # Set next player
        self.advanceToNextPlayer()

        return result

    def drawCard(self, player_id):
        """
        Player draw a card

        Parameters: player_id -> int

        Return value: dict with the following keys :
            "success"    -> bool
            "error"      -> str if success=False
            "drew_card"  -> dict {"colour": ..., "value": ...} the card drawn
        """
        current_player = self.state.players[self.state.current_turn]

        if current_player.id != player_id:
            return {"success": False, "error": "Not your turn"}

        if self.state.current_colour == "Wild":
            return {"success": False, "error": "Colour needs to be chosen"}

        print(f"\033[93m--- [MOTEUR : drawCard] ---\033[0m")
        print(f"Player number: {current_player.player_number} (ID: {player_id}) ask to draw.")
        print(f"Variables table : {self.state.current_colour} {self.state.current_value}")
        print(f"Hand in BD : {[(c.colour, c.value) for c in self._getHand(player_id)]}")
        print(f"canCurrentPlayerPlay() say : {self.canCurrentPlayerPlay()}")
        print(f"---------------------------")

        if self.canCurrentPlayerPlay() == True:
            print(f"player tried to drew while he can play")
            return {"success": False, "error": "You have at least one playable card"}

        drawn = self._pickFromDeck(1)
        if not drawn:
            if not self._reshuffleDiscardIntoDeck():
                return {"success": False, "error": "Draw pile and discard pile are empty"}
            drawn = self._pickFromDeck(1)
            if not drawn:
                return {"success": False, "error": "Draw pile is empty"}

        card          = drawn[0]
        card.location = "hand"
        card.owner_id = player_id
        self.db.commit()

        self.advanceToNextPlayer()

        return {
            "success":  True,
            "drew_card": {"colour": card.colour, "value": card.value},
        }

    def setWildColour(self, player_id, colour):
        """
        Apply chosen color after a wild

        Called AFTER platCard() when needs_color=True
        server.py has asked the player the colour and sent it here

        Parameters:
            player_id -> int
            colour    -> str : must be in ["Red", "Green", "Yellow", "Blue"]

        Return value: dict with the following keys :
            "success" -> bool
            "error"   -> str if success=False
            "colour"  -> str the chosen colour
        """
        if colour not in colours:
            return {
                "success": False,
                "error":   "Invalid colour. Choose among : {}".format(colours)
            }

        current_player = self.state.players[self.state.current_turn]
        if current_player.id != player_id:
            return {"success": False, "error": "Not your turn"}

        if self.state.current_colour != "Wild":
            return {"success": False, "error": "You did not played a card tha allow you to choose the color"}

        self.state.current_colour = colour
        self.db.commit()

        #Go to next player twice if +4
        if self.state.current_value == "Draw Four":
            self.advanceToNextPlayer()
        #Go to next player
        self.advanceToNextPlayer()

        return {"success": True, "colour": colour}

    def setScores(self):
        """
        Count remaining cards in each player's hand
        and update scores when a game ends.
        """

        totalPoints = 0
        winner = None

        for player in self.state.players:
            hand = self.getHand(player.id)
            points = 0

            for card in hand:
                points += CARD_POINTS[card["value"]]

            if player.result == "loss":
                player.score = -points
                totalPoints += points

            elif player.result == "win":
                winner = player

        if winner is not None:
            winner.score = totalPoints

        self.db.commit()



    # ──────────────────────────────────────────────────────────────────────
    # READING — Obtain game state
    # ──────────────────────────────────────────────────────────────────────

    def getHand(self, player_id):
        """
        Return player's hand

        Return value: list if dicts {"id": int, "colour": str, "value": str}

        Exemple :
            [
                {"id": 12, "colour": "Red",   "value": "7"},
                {"id": 34, "colour": "Wild",  "value": "Draw Four"},
                {"id": 56, "colour": "Blue",  "value": "Skip"},
            ]
        """
        cards = self._getHand(player_id)
        return [{"id": c.id, "colour": c.colour, "value": c.value} for c in cards]

    def getTopDiscard(self):
        """
        Return top discard card

        Return value: dict {"colour": str, "value": str} or None if discard empty
        """
        top = self._getTopDiscard()
        if not top:
            return None
        return {"colour": top.colour, "value": top.value}

    def getState(self):
        """
        Return full game state int the form of a dict

        Return value: dict with all public infos from the game
        players hands not included

        Exemple of return :
            {
                "game_id":        7,
                "status":         "playing",
                "current_turn":   2,
                "direction":      1,
                "current_colour": "Red",
                "current_value":  "7",
                "deck_size":      85,
                "top_discard":    {"colour": "Red", "value": "7"},
                "players": [
                    {"id": 1, "player_number": 0, "hand_size": 4, "result": None},
                    {"id": 2, "player_number": 1, "hand_size": 6, "result": None},
                    {"id": 3, "player_number": 2, "hand_size": 5, "result": None},
                ]
            }
        """
        deck_size = (
            self.db.query(CardModel)
            .filter(
                CardModel.game_id  == self.state.id,
                CardModel.location == "deck"
            )
            .count()
        )

        players_state = []
        for player in self.state.players:
            hand_size = (
                self.db.query(CardModel)
                .filter(
                    CardModel.game_id  == self.state.id,
                    CardModel.location == "hand",
                    CardModel.owner_id == player.id
                )
                .count()
            )
            players_state.append({
                "id":            player.id,
                "player_number": player.player_number,
                "hand_size":     hand_size,
                "result":        player.result,
                "score":         player.score,
                "player_id":     player.user_id,
            })

        return {
            "game_id":        self.state.id,
            "status":         self.state.status,
            "current_turn":   self.state.current_turn,
            "direction":      self.state.direction,
            "current_colour": self.state.current_colour,
            "current_value":  self.state.current_value,
            "deck_size":      deck_size,
            "top_discard":    self.getTopDiscard(),
            "players":        players_state,
        }

    def getCurrentPlayer(self):
        """
        Return current players turn
        Return value: GamePlayer object (SQLAlchemy model)
        """
        return self.state.players[self.state.current_turn]

    def canCurrentPlayerPlay(self):
        """
        Check if actual player has at least one card
        Return value: bool
        """
        current_player = self.getCurrentPlayer()
        hand = self._getHand(current_player.id)
        return canPlay(self.state.current_colour, self.state.current_value, hand)

    def replacePlayerWithBot(self, player_id: int):
        """Replace a human player by bot (change his flag to ai)."""
        for player in self.state.players:
            if player.id == player_id:
                player.ai = True
                player.user_id = None
                self.db.commit()
                print(f"Player {player_id} (number {player.player_number}) replaced by a bot")
                return
        print(f"⚠️ replacePlayerWithBot : player_id {player_id} not found")

    def close(self):
        """
        Return the DB connection to the pool.
        Call this when removing the game from active_games.
        """
        try:
            self.db.close()
        except Exception:
            pass
