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

void SRFRS::Deck::addCard(QSharedPointer<Flashcard> card)
{
    _flashcards.append(card);
}

void SRFRS::Deck::removeCard(QSharedPointer<Flashcard> card)
{
    _flashcards.removeAll(card);
}
