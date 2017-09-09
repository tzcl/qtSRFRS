#include "flashcardmanager.h"

#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QStringList>

SRFRS::FlashcardManager::FlashcardManager()
{

}

bool SRFRS::FlashcardManager::init(QString username, QString dirPath)
{
    _user = username;
    _dir = dirPath + "/" + _user;
    QDir directory = QDir(_dir);

    // make username folder in "AppData/Local/qtSRFRS" if folder doesn't exist
    if(!directory.exists()) directory.mkpath(directory.path());

    // load flashcards
    return load();
}

bool SRFRS::FlashcardManager::load()
{
    // load decks from file
    QFile cardFile(_dir + "/.cards");

    if(!cardFile.exists()) {
        return cardFile.open(QIODevice::WriteOnly);
    }

    if(cardFile.open(QIODevice::ReadOnly)) {
        QTextStream stream(&cardFile);
        while (!stream.atEnd()) {
            QStringList line = stream.readLine().split(";;");

            // get the flashcard data
            auto card = QSharedPointer<Flashcard>::create(line.at(0).toInt(), line.at(1), line.at(2), line.at(3), QDate::fromString(line.at(4), "dd/MM/yyyy"));
            _cards.append(card);
        }

        cardFile.close();
    } else {
        return false;
    }

    return true;
}

void SRFRS::FlashcardManager::addFlashcard(QSharedPointer<Flashcard> card)
{
    // add to _cards
    _cards.append(card);

    // write to .cards
    QFile cardsFile(_dir + "/.cards");

    if (cardsFile.open(QIODevice::Append)) {

        QTextStream stream(&cardsFile);

        stream << QString::number(card->getID()) + ";;";
        stream << card->getFront() + ";;";
        stream << card->getBack() + ";;";
        stream << card->getDeck() + ";;";
        stream << card->getDate().toString("dd/MM/yyyy");
        stream << endl;

        cardsFile.close();
    }
}

void SRFRS::FlashcardManager::setID(int index, int oldId, int id)
{
    // set id
    _cards[index]->setID(id);

    // update .cards
    update(oldId, 0, QString::number(id));
}

void SRFRS::FlashcardManager::removeFlashcard(int id)
{
    auto p_card = getFlashcard(id);

    // remove card from _cards
    if(_cards.contains(p_card)) {
        _cards.removeAll(p_card);
    } else {
        qDebug() << "_cards doesn't contain " + id;
    }

    // remove card from .cards
    QFile cardsFile(_dir + "/.cards");

    if(cardsFile.open(QIODevice::ReadWrite)) {

        QString oldContents;
        QTextStream newContents(&cardsFile);

        while(!newContents.atEnd()){

            QStringList line = newContents.readLine().split(";;");

            if(line.at(0) != QString::number(id)) {
                oldContents.append(line.join(";;") + "\n");
            }
        }

        cardsFile.resize(0);
        newContents << oldContents;

        cardsFile.close();
    }
}

QSharedPointer<SRFRS::Flashcard> SRFRS::FlashcardManager::getFlashcard(int id)
{
    for(int i = 0; i < _cards.size(); ++i) {
        if(_cards[i]->getID() == id) return _cards[i];
    }

    qDebug() << "couldn't find flashcard :S in flashcardmanager getFlashcard!! UNDEFINED BEHAVIOUR AHEAD";
    return _cards[0];
}

void SRFRS::FlashcardManager::update(int id, int index, QString after)
{
    // only 4 possible data entries: id, front, back, deck, date
    // therefore index must be between 0 and 3
    if(index < 0 || index > 3) {
        qDebug() << "index out of range!! " + QString::number(index) + " id: " + QString::number(id);
        return;
    }

    // update cards file
    QFile deckFile(_dir + "/.cards");

    if(deckFile.open(QIODevice::ReadWrite)) {

        QString existingText;
        QTextStream stream(&deckFile);

        while(!stream.atEnd()){

            QString line = stream.readLine();
            QStringList parts = line.split(";;");

            if(parts.at(0) == QString::number(id)) {
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
