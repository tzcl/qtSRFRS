#ifndef DECK_H
#define DECK_H

#include <QVector>
#include <QSharedPointer>
#include <QString>
#include <QDate>

#include "flashcard.h"

namespace SRFRS {

class Deck
{
public:
    Deck();
    explicit Deck(QString name);

    // getter for _name
    QString getName() { return _name; }

    // setter for _name
    void setName(QString name) { _name = name; }

    /**
     * @brief addCard: Add a flashcard to the deck's vector of flashcards.
     * @param card: The flashcard to add to the deck.
     */
    void addCard(QSharedPointer<Flashcard> card);

    /**
     * @brief removeCard: Remove a flashcard from the deck's vector of flashcards.
     * @param card: The flashcard to remove from the deck.
     */
    void removeCard(QSharedPointer<Flashcard> card);

    /**
     * @brief addReviewCard: Add a flashcard to the deck's list of flashcards
     *                       that need to be reviewed.
     * @param card: The flashcard to add to the deck's review list.
     */
    void addReviewCard(QSharedPointer<Flashcard> card);

    /**
     * @brief removeReviewCard: Remove a flashcard from the deck's list of flashcards
     *                          that need to be reviewed.
     * @param card: The flashcard to add to the deck's review list.
     */
    void removeReviewCard(QSharedPointer<Flashcard> card);

    /**
     * @brief calculateDueDate: Find the earliest due date for flashcards in the deck
     *                          and set the deck's due date to that.
     */
    void calculateDueDate();

    // getter for _flashcards
    QVector<QSharedPointer<Flashcard>> getVector() { return _flashcards; }

    // getter for _review
    QVector<QSharedPointer<Flashcard>> getReview() { return _review; }

    // returns the number of flashcards
    QString getFlashcards() { return QString::number(_flashcards.size()); }

    // getter for _date
    QDate getDate() { return _date; }

    // setter for _date
    void setDate(QDate date) { _date = date; }

private:
    QString _name;
    QVector<QSharedPointer<Flashcard>> _flashcards;
    QVector<QSharedPointer<Flashcard>> _review;
    QDate _date;
};

}

#endif // DECK_H
