#include "deckmanager.h"

#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QFile>

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

SRFRS::Deck SRFRS::DeckManager::getDeck(QString deckName)
{
    for(int i = 0; i < _collection.getDecks().size(); ++i) {
        if(_collection.getDecks()[i].getName() == deckName) {
            return _collection.getDecks()[i];
        }
    }

    qDebug() << "couldn't find deck :S in deckmanager getDeck";
    return _collection.getDecks()[0];
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

void SRFRS::DeckManager::removeDeck(QString deckName)
{
    _collection.removeDeck(getDeck(deckName));
}

void SRFRS::DeckManager::renameDeck(QString oldName, Deck &deck)
{
    _collection.renameDeck(oldName, deck);
}

void SRFRS::DeckManager::update(QString deckName, int index, QString after)
{
    // update decks file
    QFile deckFile(_dirPath + "/.decks");

    if(deckFile.open(QIODevice::ReadWrite)) {

        QString existingText;
        QTextStream stream(&deckFile);

        while(!stream.atEnd()){

            QString line = stream.readLine();
            QStringList parts = line.split(";;");

            if(parts.at(0) == deckName) {
                parts[index] = after;
            }

            line = parts.join(";;");

            existingText.append(line + "\n");
        }

        deckFile.resize(0);
        stream << existingText;

        deckFile.close();
    }
}
