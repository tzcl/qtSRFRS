#ifndef FLASHCARDMANAGER_H
#define FLASHCARDMANAGER_H

#include "flashcard.h"

#include <QVector>
#include <QString>
#include <QSharedPointer>

namespace SRFRS {

class FlashcardManager
{
public:
    FlashcardManager();

    bool init(QString user, QString dir);

    void addFlashcard(QSharedPointer<Flashcard> card);

    void removeFlashcard(int id);

    void setID(int index, int oldID, int ID);

    int getValidID() { return _cards.size(); }

    void renameFiles(int oldId, int id);

    void updateFront(int id, QString after);

    void updateBack(int id, QString after);

    void update(int id, int index, QString after);

    QSharedPointer<Flashcard> getFlashcard(int id);

    QVector<QSharedPointer<Flashcard>> getFlashcards() { return _cards; }

private:

    bool load();

    QString _user;
    QString _dir;

    QVector<QSharedPointer<Flashcard>> _cards;
};

}
#endif // FLASHCARDMANAGER_H
