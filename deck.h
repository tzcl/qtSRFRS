#ifndef DECK_H
#define DECK_H

#include <QString>
#include <QVector>
#include <QDate>
#include <QDebug>

#include "flashcard.h"

namespace SRFRS {

class Deck
{
public:
    Deck();
    Deck(QString name, int flashcards, QDate date);

    void remove(bool remove) { _remove = remove; }

    void addCard();

    void removeCard();

    QString getName() const { return _name; }
    void setName(QString name) { _name = name; }

    QString getFlashcards() const { return QString::number(_flashcards); }

    QDate getDate() const { return _date; }
    void setDate(QString date) { _date = QDate::fromString(date, "d/M/yyyy"); }


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
    int _flashcards;
    QDate _date;

    bool _remove;
};

}

#endif // DECK_H
