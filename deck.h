#ifndef DECK_H
#define DECK_H

#include <QString>
#include <QVector>

#include "flashcard.h"

namespace SRFRS {

class Deck
{
public:
    Deck(QString name);

    /**
     * @brief addCards: Adds new cards to the deck, only if they are not already in the deck.
     * @param add_cards: Reference to a list of flashcards to add.
     */
    void addCards(QVector<Flashcard> &add_cards);

    /**
     * @brief removeCards: Removes cards from the deck, given they are already in the deck.
     * @param remove_cards: Reference to a list of flashcards to remove.
     */
    void removeCards(QVector<Flashcard> &remove_cards);

    /**
     * @brief renameDeck: Rename the deck.
     * @param name: The new name.
     */
    void renameDeck(QString name);

private:
    QString _name;
    QVector<Flashcard> _flashcards;
};

}

#endif // DECK_H
