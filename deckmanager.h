#ifndef DECKMANAGER_H
#define DECKMANAGER_H

#include <QVector>
#include <QDir>
#include <QTableWidget>
#include <QWidget>
#include <QStringList>

#include "collection.h"

namespace SRFRS {

class DeckManager
{

public:
    DeckManager();

    bool init(QString username, QString dirPath);

    void addDeck(Deck &deck);

    void removeDeck(int row);

    QStringList deckNames();

    Collection getCollection() { return _collection; }

private: 

    QString _user;
    QString _dirPath;
    QDir _dir;

    Collection _collection;
};

}

#endif // DECKMANAGER_H
