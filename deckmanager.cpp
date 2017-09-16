#include "deckmanager.h"

#include <QDebug>
#include <QString>
#include <QTextStream>
#include <QFile>
#include <QDir>

SRFRS::DeckManager::DeckManager() :
    _user(),
    _dir(),
    _decks()
{

}

bool SRFRS::DeckManager::init(QString username, QString dirPath)
{
    _user = username;
    _dir = dirPath + "/" + _user + "/decks";
    QDir directory = QDir(_dir);

    // make username folder in "AppData/Local/qtSRFRS" if folder doesn't exist
    if(!directory.exists()) directory.mkpath(directory.path());

    // load decks
    return load();
}

bool SRFRS::DeckManager::load()
{
    QFile deckFile(_dir + "/.decks");

    if(!deckFile.exists()) {
        return deckFile.open(QIODevice::WriteOnly);
    }

    if(deckFile.open(QIODevice::ReadOnly)) {
        QTextStream stream(&deckFile);
        while (!stream.atEnd()) {
            QStringList line = stream.readLine().split(";;");

            // get the deck data
            auto deck = QSharedPointer<Deck>::create(line.at(0), QDate::fromString(line.at(1), "dd/MM/yyyy"));
            _decks.append(deck);
        }

        deckFile.close();
    } else {
        return false;
    }

    return true;
}

void SRFRS::DeckManager::addDeck(QSharedPointer<Deck> deck)
{
    // add to decks
    _decks.append(deck);

    // write to .decks
    QFile deckFile(_dir + "/.decks");

    if (deckFile.open(QIODevice::Append)) {

        QTextStream stream(&deckFile);

        stream << deck->getName() + ";;" << deck->getDate().toString("dd/MM/yyyy") << endl;

        deckFile.close();
    }
}


void SRFRS::DeckManager::renameDeck(QString oldName, QString newName)
{
    // rename in decks file
    update(oldName, 0, newName);
}

void SRFRS::DeckManager::removeDeck(QString deckName)
{
    auto p_deck = getDeck(deckName);

    // remove deck from _decks
    if(_decks.contains(p_deck)) {
        _decks.removeAll(p_deck);
    } else {
        qDebug() << "_decks doesn't contain " + deckName;
    }

    // remove deck from .decks
    QFile deckFile(_dir + "/.decks");

    if(deckFile.open(QIODevice::ReadWrite)) {

        QString oldContents;
        QTextStream newContents(&deckFile);

        while(!newContents.atEnd()){

            QStringList line = newContents.readLine().split(";;");

            if(line.at(0) != deckName) {
                oldContents.append(line.join(";;") + "\n");
            }
        }

        deckFile.resize(0);
        newContents << oldContents;

        deckFile.close();
    }
}

QStringList SRFRS::DeckManager::getDeckNames()
{
    QStringList result;

    for(int i = 0; i < _decks.size(); ++i) {
        result.append(_decks[i]->getName());
    }

    return result;
}

QSharedPointer<SRFRS::Deck> SRFRS::DeckManager::getDeck(QString deckName)
{
    for(int i = 0; i < _decks.size(); ++i) {
        if(_decks[i]->getName() == deckName) {
            return _decks[i];
        }
    }

    qDebug() << "couldn't find deck :S in deckmanager getDeck!! UNDEFINED BEHAVIOUR AHEAD. looking for:" << deckName;
    return _decks[0];
}

void SRFRS::DeckManager::update(QString deckName, int index, QString after)
{
    // only 2 possible data entries: name, date last reviewed
    // therefore index must be between 0 and 1
    if(index < 0 || index > 1) {
        qDebug() << "index out of range!! " + QString::number(index) + " deck: " + deckName;
        return;
    }

    // update cards file
    QFile deckFile(_dir + "/.decks");

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
