#include "flashcard.h"

// need to have front/back (text/images/audio)

SRFRS::Flashcard::Flashcard() :
    _ID(0),
    _frontText(""),
    _backText(""),
    _dateCreated(QDate::currentDate())
{

}

SRFRS::Flashcard::Flashcard(int ID, QString front, QString back, QDate date) :
    _ID(ID),
    _frontText(front),
    _backText(back),
    _dateCreated(date)
{

}
