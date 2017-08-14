#ifndef COLLECTION_H
#define COLLECTION_H

#include <QVector>

#include "deck.h"

namespace SRFRS {

class Collection
{
public:
    Collection();

    bool load(QString dir);

    bool save();

    bool addDeck(Deck &deck);

    bool removeDeck(Deck &deck);

    // TODO: wrap this
    QVector<Deck> getDecks() { return _decks; }

private:

    QString _dir;

    QVector<Deck> _decks;

};

}

#endif // COLLECTION_H
