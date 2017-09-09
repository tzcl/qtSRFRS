#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QString>
#include <QDate>

namespace SRFRS {

class Flashcard
{
public:
    Flashcard();
    Flashcard(int ID, QString front, QString back, QString deck, QDate date);

    int getID() { return _ID; }

    void setID(int id) { _ID = id; }

    QString getFront() { return _frontText; }

    QString getBack() { return _backText; }

    QString getDeck() { return _deck; }

    void setDeck(QString deck) { _deck = deck; }

    QDate getDate() { return _dateCreated; }

    /**
     * @brief operator ==: Overload the binary equality operator.
     * @param lhs: First flashcard to compare.
     * @param rhs: Second flashcard to compare.
     * @return: True if the front and back of both flashcards match, false otherwise.
     */
    friend bool operator==(const Flashcard& lhs, const Flashcard& rhs) {
        return lhs._frontText == rhs._frontText && lhs._backText == rhs._backText;
    }

private:

    int _ID;

    // card front
    QString _frontText;

    // card back
    QString _backText;

    QString _deck;

    QDate _dateCreated;
};

}

#endif // FLASHCARD_H
