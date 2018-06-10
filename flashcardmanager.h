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

    /**
     * @brief init: Initialise the flashcard manager and load in all user flashcards from file.
     * @param user: The current user.
     * @param dir: The address of the qtSRFRS directory in AppData.
     * @return: True if initialise is successful, false otherwise.
     */
    bool init(QString user, QString dir);

    /**
     * @brief addFlashcard: Adds the flashcard to memory and to file.
     * @param card: The flashcard to add.
     */
    void addFlashcard(QSharedPointer<Flashcard> card);

    /**
     * @brief removeFlashcard: Removes the flashcard from memory and from file.
     * @param id: The flashcard to remove.
     */
    void removeFlashcard(int id);

    /**
     * @brief setID: Sets the ID of the flashcard at index in memory from oldID to ID.
     * @param index: The index of the flashcard in memory.
     * @param oldID: The oldID of the flashcard.
     * @param ID: The new ID of the flashcard.
     */
    void setID(int index, int oldID, int ID);

    /**
     * @brief getValidID: Returns the next free memory index.
     * @return: The next free memory index.
     */
    int getValidID() { return _cards.size(); }

    /**
     * @brief renameFiles: Renames the flashcard's front and back files from oldId to id.
     * @param oldId: The old ID of the flashcard.
     * @param id: The new ID of the flashcard.
     */
    void renameFiles(int oldId, int id);

    /**
     * @brief updateFront: Update the front of the flashcard with ID id.
     * @param id: The ID of the flashcard.
     * @param after: The new front of the flashcard.
     */
    void updateFront(int id, QString after);

    /**
     * @brief updateBack: Update the back of the flashcard with ID id.
     * @param id: The ID of the flashcard.
     * @param after: The new back of the flashcard.
     */
    void updateBack(int id, QString after);

    /**
     * @brief schedule: Determines the optimal time to next review the flashcard and sets the
     *                  flashcard's due date to this date.
     * @param id: The ID of the flashcard.
     * @param difficulty: The user difficulty when reviewing this flashcard.
     */
    void schedule(int id, int difficulty);

    /**
     * @brief update: Updates a value of the flashcard with ID id in the flashcards file.
     * @param id: The ID of the flashcard.
     * @param index: Which element of the flashcard to update.
     * @param after: The updated value of the flashcard element.
     */
    void update(int id, int index, QString after);

    /**
     * @brief getFlashcard: Returns the flashcard with the specified ID.
     * @param id: The ID to search for.
     * @return: A pointer to the flashcard with the specified ID.
     */
    QSharedPointer<Flashcard> getFlashcard(int id);

    /**
     * @brief getFlashcards: Returns the entire list of user flashcards.
     * @return: List of all user flashcards.
     */
    QVector<QSharedPointer<Flashcard>> getFlashcards() { return _cards; }

private:

    bool load();

    QString _user;
    QString _dir;

    QVector<QSharedPointer<Flashcard>> _cards;
};

}
#endif // FLASHCARDMANAGER_H
