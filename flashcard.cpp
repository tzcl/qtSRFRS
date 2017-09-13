#include "flashcard.h"

SRFRS::Flashcard::Flashcard() :
    _ID(0),
    _frontText(),
    _backText(),
    _deck(""),
    _dateCreated(QDate::currentDate())
{

}

SRFRS::Flashcard::Flashcard(int ID, QStringList front, QStringList back, QString deck, QDate date) :
    _ID(ID),
    _frontText(front),
    _backText(back),
    _deck(deck),
    _dateCreated(date)
{

}
