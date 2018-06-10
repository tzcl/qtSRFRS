#include "flashcard.h"

SRFRS::Flashcard::Flashcard() :
    _ID(0),
    _deck(""),
    _reviewDate(QDate::currentDate()),
    _frontText(),
    _backText(),
    _previousInterval(),
    _difficultyCoefficient()
{

}

SRFRS::Flashcard::Flashcard(int ID, QStringList front, QStringList back, QString deck, QDate reviewDate, int repetitions, int previousInterval, float difficultyCoefficient) :
    _ID(ID),
    _deck(deck),
    _reviewDate(reviewDate),
    _frontText(front),
    _backText(back),
    _repetitions(repetitions),
    _previousInterval(previousInterval),
    _difficultyCoefficient(difficultyCoefficient)
{

}
