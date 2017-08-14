#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QString>

namespace SRFRS {

class Flashcard
{
public:
    Flashcard();
    Flashcard(QString front, QString back);

    QString getFront() { return _frontText; }

    QString getBack() { return _backText; }

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
    // card front
    QString _frontText;

    // card back
    QString _backText;
};

}

#endif // FLASHCARD_H
