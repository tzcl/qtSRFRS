#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QString>

namespace SRFRS {

class Flashcard
{
public:
    Flashcard();
    Flashcard(QString front, QString back);

    friend bool operator==(const Flashcard& lhs, const Flashcard& rhs) {
        return lhs._frontText == rhs._frontText && lhs._backText == rhs._backText;
    }

private:
    // card back
    QString _backText;

    // card front
    QString _frontText;
};

}

#endif // FLASHCARD_H
