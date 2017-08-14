#include "deckmanager.h"

#include <QDebug>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QToolButton>
#include <QSignalMapper>
#include <QString>

#include "deckeditor.h"

SRFRS::DeckManager::DeckManager() :
    _user(),
    _dirPath(),
    _dir(),
    _collection()
{

}

bool SRFRS::DeckManager::init(QString username, QString dirPath)
{
    _user = username;
    _dirPath = dirPath + "/" + _user;
    _dir = QDir(_dirPath);

    // make username folder in "AppData/Local/qtSRFRS" if folder doesn't exist
    if(!_dir.exists()) _dir.mkpath(_dir.path());

    // load collection
    return _collection.load(_dirPath);
}

QStringList SRFRS::DeckManager::deckNames()
{
    QStringList result;

    for(int i = 0; i < _collection.getDecks().size(); ++i) {
        result.append(_collection.getDecks().at(i).getName());
    }

    return result;
}

void SRFRS::DeckManager::addDeck(Deck& deck)
{
    _collection.addDeck(deck);
}


void SRFRS::DeckManager::removeDeck(int row)
{
    _collection.removeDeck(_collection.getDecks()[row]);
}
