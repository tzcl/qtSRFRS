#include "deck.h"

#include <QDebug>

SRFRS::Deck::Deck() :
    _name("default"),
    _flashcards(),
    _review(),
    _date(QDate::currentDate())
{

}

SRFRS::Deck::Deck(QString name) :
    _name(name),
    _flashcards(),
    _review(),
    _date(QDate::currentDate())
{

}

void SRFRS::Deck::addCard(QSharedPointer<Flashcard> card)
{
    // add the card to _flashcards
    _flashcards.append(card);

    // check if card due date is earlier or equal to today
    if(card->getReviewDate() <= QDate::currentDate()) {
        // add card to _review
        _review.append(card);
    }

    // recalculate deck due date
    calculateDueDate();
}

void SRFRS::Deck::removeCard(QSharedPointer<Flashcard> card)
{
    // remove card from _flashcards
    _flashcards.removeAll(card);

    // check if card is in _review, then remove it from there too
    if(_review.contains(card)) _review.removeAll(card);

    // recalculate deck due date
    calculateDueDate();
}

void SRFRS::Deck::addReviewCard(QSharedPointer<Flashcard> card)
{
    // add card to _review
    _review.append(card);

    // recalculate deck due date
    calculateDueDate();
}

void SRFRS::Deck::removeReviewCard(QSharedPointer<Flashcard> card)
{
    // check if card still needs to be reviewed or not
    if(card->getReviewDate() > QDate::currentDate()) {
        // remove it from _review
        _review.removeAll(card);
    }

    // recalculate deck due date
    calculateDueDate();
}

void SRFRS::Deck::calculateDueDate()
{
    // make sure _flashcards isn't empty
    if(!_flashcards.isEmpty()) {

        // assign earliestDate to the first flashcard by default
        QDate earliestDate = _flashcards[0]->getReviewDate();

        for(int j = 0; j < _flashcards.size(); ++j) {

            // check if any flashcards are due earlier than earliestDate
            if(_flashcards[j]->getReviewDate() < earliestDate) {
                // update earliestDate to the due date of the flashcard
                earliestDate = _flashcards[j]->getReviewDate();
            }
        }

        // update the deck _date value to earliestDate
        _date = earliestDate;
    }
}
