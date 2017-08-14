#include "deck.h"

#include <QDebug>

SRFRS::Deck::Deck() :
    _name(""),
    _flashcards()
{

}

SRFRS::Deck::Deck(QString name) :
    _name(name),
    _flashcards()
{
    qDebug() << _flashcards.size();
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

void SRFRS::Deck::renameDeck(QString name)
{
    _name = name;
}
