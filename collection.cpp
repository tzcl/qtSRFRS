#include "collection.h"

#include <QTextStream>
#include <QFile>
#include <QStringList>
#include <QString>
#include <QDebug>
#include <QDate>

SRFRS::Collection::Collection() :
    _dir(),
    _decks()
{

}

bool SRFRS::Collection::load(QString dir)
{
    // save the directory
    _dir = dir;

    // load decks from file
    QFile deckFile(_dir + "/.decks");

    if(!deckFile.exists()) {
        return deckFile.open(QIODevice::WriteOnly);
    }

    if(deckFile.open(QIODevice::ReadOnly)) {
        QTextStream stream(&deckFile);
        while (!stream.atEnd()) {
            QStringList line = stream.readLine().split(";;");

            // get the deck data
            Deck deck(line.at(0), line.at(1).toInt(), QDate::fromString(line.at(2), "dd/MM/yyyy"));
            _decks.append(deck);
        }

        deckFile.close();
    } else {
        return false;
    }

    return true;
}

bool SRFRS::Collection::save()
{
    // save to decks file
    QFile deckFile(_dir + "/.decks");

    if(deckFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream stream(&deckFile);

        for(int i = 0; i < _decks.size(); ++i) {
            stream << _decks.at(i).getName() + ";;";
            stream << _decks.at(i).getFlashcards() + ";;";
            stream << _decks.at(i).getDate().toString("dd/MM/yyyy");
            stream << endl;
        }

        deckFile.close();
    } else {
        return false;
    }

    return true;
}

bool SRFRS::Collection::addDeck(Deck &deck)
{
    _decks.append(deck);

    // add to decks file
    QFile deckFile(_dir + "/.decks");

    if (deckFile.open(QIODevice::Append)) {

        QTextStream stream(&deckFile);

        stream << deck.getName() + ";;";
        stream << deck.getFlashcards() + ";;";
        stream << deck.getDate().toString("dd/MM/yyyy");
        stream << endl;

        deckFile.close();
    } else {
        return false;
    }

    return true;
}

bool SRFRS::Collection::removeDeck(Deck &deck)
{
    _decks.removeOne(deck);

    // remove from decks file
    QFile deckFile(_dir + "/.decks");

    if(deckFile.open(QIODevice::ReadWrite)) {

        QString existingText;
        QTextStream stream(&deckFile);

        while(!stream.atEnd()){

            QString line = stream.readLine();
            if(!line.contains(deck.getName())) {
                existingText.append(line + "\n");
            }
        }

        deckFile.resize(0);
        stream << existingText;

        deckFile.close();
    } else {
        return false;
    }

    return true;
}
