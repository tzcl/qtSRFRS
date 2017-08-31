#ifndef FLASHCARDMANAGER_H
#define FLASHCARDMANAGER_H

#include "flashcard.h"

#include <QVector>
#include <QString>
#include <QDir>

namespace SRFRS {

class FlashcardManager
{
public:
    FlashcardManager();

    bool init(QString username, QString dirPath);

    bool save();

    bool addFlashcard(Flashcard &card);

    bool removeFlashcard(Flashcard &card);

    void setID(int index, int oldId, int id);

    Flashcard getFlashcard(int ID);

    QVector<Flashcard> getFlashcards() { return _cards; }

private:

    bool load();

    QString _dirPath;
    QString _user;
    QDir _dir;

    QVector<Flashcard> _cards;
};

}
#endif // FLASHCARDMANAGER_H
