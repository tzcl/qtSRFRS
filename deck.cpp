#include "deck.h"

#include <QDebug>

SRFRS::Deck::Deck() :
    _name("default"),
    _flashcards(),
    _date(QDate::currentDate())
{

}

SRFRS::Deck::Deck(QString name, int flashcards, QDate date) :
    _name(name),
    _flashcards(flashcards),
    _date(date)
{

}
