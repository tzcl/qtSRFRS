#ifndef DECKMANAGER_H
#define DECKMANAGER_H

#include <QVector>
#include <QSharedPointer>
#include <QDir>
#include <QStringList>

#include "deck.h"

namespace SRFRS {

class DeckManager
{

public:
    DeckManager();

    bool init(QString username, QString dirPath);

    void addDeck(QSharedPointer<Deck> deck);

    void renameDeck(QString oldName, QString newName);

    void removeDeck(QString deckName);

    QStringList getDeckNames();

    QSharedPointer<Deck> getDeck(QString deckName);

    QVector<QSharedPointer<Deck>> getDecks() { return _decks; }

    void update(QString deckName, int index, QString after);

private: 

    bool load();

    // read/write to .decks
    QString _user;
    QString _dir;

    QVector<QSharedPointer<Deck>> _decks;
};

}

#endif // DECKMANAGER_H
