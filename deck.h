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
    explicit Deck(QString name, QDate date);

    QString getName() { return _name; }

    void addCard(QSharedPointer<Flashcard> card);

    void removeCard(QSharedPointer<Flashcard> card);

    QVector<QSharedPointer<Flashcard>> getVector() { return _flashcards; }

    QString getFlashcards() { return QString::number(_flashcards.size()); }

    QDate getDate() { return _date; }

    void setName(QString name) { _name = name; }

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
    QVector<QSharedPointer<Flashcard>> _flashcards;
    QDate _date;
};

}

#endif // DECK_H
