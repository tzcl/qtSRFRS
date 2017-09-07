#include "deck.h"

#include <QDebug>

SRFRS::Deck::Deck() :
    _name(""),
    _flashcards(0),
    _date(QDate()),
    _remove(false)
{

}

SRFRS::Deck::Deck(QString name, int flashcards, QDate date) :
    _name(name),
    _flashcards(flashcards),
    _date(date),
    _remove(false)
{

}

void SRFRS::Deck::addCard()
{
    _flashcards++;
}

void SRFRS::Deck::removeCard()
{
//    if(_flashcards == 0) {
//        qDebug() << "0 flashcards";
//    } else {
//        _flashcards--;
//    }
}
