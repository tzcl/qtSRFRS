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

void SRFRS::Deck::addCards(QVector<Flashcard> &add_cards)
{
    for (int i = 0; i < add_cards.size(); ++i) {
        if(!_flashcards.contains(add_cards.at(i))) _flashcards.append(add_cards.at(i));
    }
}

void SRFRS::Deck::removeCards(QVector<Flashcard> &remove_cards)
{
    for(int i = 0; i < remove_cards.size(); ++i) {
        if(_flashcards.contains(remove_cards.at(i))) _flashcards.remove(i);
    }
}
