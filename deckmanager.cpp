#include "deckmanager.h"

SRFRS::DeckManager::DeckManager() :
    _decks()
{
    // load user decks
}

void SRFRS::DeckManager::addDeck(Deck &deck)
{
    _decks.append(deck);
}

void SRFRS::DeckManager::removeDeck(Deck &deck)
{
    _decks.removeOne(deck);
}
