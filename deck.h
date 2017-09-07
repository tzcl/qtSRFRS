#ifndef DECK_H
#define DECK_H

#include <QVector>
#include <QSharedPointer>
#include <QString>
#include <QDate>
#include <QDebug>

namespace SRFRS {

class Deck
{
public:
    Deck();
    explicit Deck(QString name, int flashcards, QDate date);

    QString getName() { return _name; }

    QString getFlashcards() { return QString::number(_flashcards); }

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
    int _flashcards;
    QDate _date;
};

}

#endif // DECK_H
