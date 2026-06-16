"""
utils.py
utils fonction that do not interact with the db
"""

import random
from models import Card, colours
from display import bplayers, butils


# ─────────────────────────────────────────────
# Construction and shuffle of the deck
# ─────────────────────────────────────────────

def buildDeck():
    """
    Generate all 108 uno cars
    Return a Card objects list (not in DB)
    Parameters: None
    Return value: deck -> list[Card]
    """
    deck = []
    values = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, "Draw Two", "Skip", "Reverse"]

    for colour in colours:
        for value in values:
            deck.append(Card(colour=colour, value=str(value), location="deck"))
            if value != 0:
                deck.append(Card(colour=colour, value=str(value), location="deck"))

    for _ in range(4):
        deck.append(Card(colour="Wild", value="Any",       location="deck"))
        deck.append(Card(colour="Wild", value="Draw Four", location="deck"))

    return deck


def shuffleDeck(deck):
    """
    Shuffle a list (deck)
    Parameters: deck -> list
    Return value: deck -> list
    """
    size = len(deck)
    for cardPos in range(size):
        randPos = random.randint(0, size - 1)
        deck[cardPos], deck[randPos] = deck[randPos], deck[cardPos]
    return deck


# ─────────────────────────────────────────────
# Reading hand and discard
# ─────────────────────────────────────────────

def showHand(playerNumber, playerHand):
    """
    Depricated fuction, was used for terminal use
    Print player hand
    Parameters: playerNumber -> int, playerHand -> list[Card]
    Return value: None
    """
    colour = bplayers.LIST[playerNumber % len(bplayers.LIST)]
    print(colour + "Player {}'s Turn".format(playerNumber + 1) + butils.RESET)
    print("Your Hand")
    print("---------------")
    for i, card in enumerate(playerHand, start=1):
        print("{}) {}".format(i, card))
    print("")


def updateCurrentDiscard(discard):
    """
    Update discard card
    Parameters: discard -> Card
    Return value: currentColour -> str, cardVal -> str
    """
    currentColour = discard.colour
    cardVal       = discard.value
    return currentColour, cardVal


# ─────────────────────────────────────────────
# Rules and settings
# ─────────────────────────────────────────────

def canPlay(colour, value, playerHand):
    """
    Check if at least one card is playable in the hand
    Parameters: colour -> str, value -> str, playerHand -> list[Card]
    Return value: bool
    """
    for card in playerHand:
        if card.colour == "Wild":
            print(f"card is a wild and thus can be played")
            return True
        if card.colour == colour or card.value == value:
            return True
    return False


def setNextPlayer(playerTurn, numPlayers, playDirection):
    """
    Find next player
    Parameters: playerTurn -> int, numPlayers -> int, playDirection -> int (1 ou -1)
    Return value: playerTurn -> int
    """
    return (playerTurn + playDirection) % numPlayers
