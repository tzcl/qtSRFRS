#ifndef DECK_H
#define DECK_H

#include <QString>
#include <QVector>

#include "flashcard.h"

namespace SRFRS {

class Deck
{
public:
    Deck();
    Deck(QString name);

    /**
     * @brief addCard: Adds the card to the deck, given it is not already in the deck.
     * @param card: The card to add.
     */
    void addCard(Flashcard &card);

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

    /**
     * @brief operator ==: Overload the binary equality operator.
     * @param lhs: First deck to compare.
     * @param rhs: Second deck to compare.
     * @return: True if decks have the same name, false otherwise.
     */
    friend bool operator==(const Deck& lhs, const Deck& rhs) {
        return lhs._name == rhs._name;
    }

private:
    QString _name;
    QVector<Flashcard> _flashcards;
};

}

#endif // DECK_H
