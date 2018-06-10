#ifndef DECKMANAGER_H
#define DECKMANAGER_H

#include <QVector>
#include <QSharedPointer>
#include <QDir>
#include <QStringList>

#include "deck.h"

namespace SRFRS {

class DeckManager
{

public:
    DeckManager();

    /**
     * @brief init: Initialises the deck manager and loads in all user decks from file.
     * @param user: The current user.
     * @param dir: The address of the qtSRFRS directory in AppData.
     * @return: True if initialise is successful, false otherwise.
     */
    bool init(QString username, QString dirPath);

    /**
     * @brief addDeck: Adds the deck to user and to memory.
     * @param deck: The deck to add.
     */
    void addDeck(QSharedPointer<Deck> deck);

    /**
     * @brief renameDeck: Renames the deck from oldName to newName.
     * @param oldName: The old name of the deck.
     * @param newName: The new name of the deck.
     */
    void renameDeck(QString oldName, QString newName);

    /**
     * @brief removeDeck: Removes the deck from memory and from file.
     * @param deckName: The name of the deck to remove.
     */
    void removeDeck(QString deckName);

    /**
     * @brief getDeckNames: Returns a list of all user deck names.
     * @return: A list of all user deck names.
     */
    QStringList getDeckNames();

    /**
     * @brief getDeck: Returns a pointer to the deck with name deckName.
     * @param deckName: The name of the deck to search for.
     * @return: A pointer to the deck with name deckName.
     */
    QSharedPointer<Deck> getDeck(QString deckName);

    /**
     * @brief getDecks: Returns a list of all user decks.
     * @return: A list of all user decks.
     */
    QVector<QSharedPointer<Deck>> getDecks() { return _decks; }

private: 

    bool load();

    // read/write to .decks
    QString _user;
    QString _dir;

    QVector<QSharedPointer<Deck>> _decks;
};

}

#endif // DECKMANAGER_H
