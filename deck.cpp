#include "deck.h"

SRFRS::Deck::Deck(QString name) :
    _name(name),
    _flashcards()
{

}

void SRFRS::Deck::addCards(QVector<Flashcard> &add_cards)
{
//    for(const auto& card : add_cards) {
//        if(!_flashcards.contains(card)) _flashcards.append(card);
//    }
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
