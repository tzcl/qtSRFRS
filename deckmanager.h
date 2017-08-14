#ifndef DECKMANAGER_H
#define DECKMANAGER_H

#include <QVector>

#include "deck.h"

namespace SRFRS {

class DeckManager
{
public:
    DeckManager();

    void addDeck(Deck& deck);

    void removeDeck(Deck& deck);

private:
    // list of user decks
    QVector<Deck> _decks;
};

}

#endif // DECKMANAGER_H
