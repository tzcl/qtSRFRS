#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "loginwindow.h"
#include "accountmanager.h"
#include "deckmanager.h"
#include "deck.h"
#include "flashcardmanager.h"

#include <QMainWindow>
#include <QPushButton>
#include <QStringList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString username, QString dirPath, SRFRS::AccountManager accountManager, LoginWindow *loginWindow, QWidget *parent = 0);
    ~MainWindow();

    /**
     * @brief getAccountManager: Returns the account manager instance.
     * @return: The account manager instance.
     */
    SRFRS::AccountManager getAccountManager() { return _accountManager; }

    /**
     * @brief getDeckManager: Returns the deck manager instance.
     * @return: The deck manager instance.
     */
    SRFRS::DeckManager getDeckManager() { return _deckManager; }

    /**
     * @brief getFlashcardManager: Returns the flashcard manager instance.
     * @return: The flashcard manager instance.
     */
    SRFRS::FlashcardManager getFlashcardManager() { return _flashcardManager; }

    /**
     * @brief getUser: Returns the current user's username.
     * @return: The current user's username.
     */
    QString getUser() { return _username; }

    /**
     * @brief getPath: Returns the path to the qtSRFRS AppData folder
     * @return: The path to the qtSRFRS AppData folder
     */
    QString getPath() { return _dirPath; }

    /**
     * @brief getDecksToReview: Returns a list of the names of decks that need to be reviewed.
     * @return: The current list of the names of decks that need to be reviewed.
     */
    QStringList getDecksToReview() { return _decksToReview; }

    /**
     * @brief getUI: Returns the window's ui pointer.
     * @return: The window's ui pointer.
     */
    Ui::MainWindow* getUI() { return ui; }

    // deck methods

    /**
     * @brief addDeck: Adds a deck to the system.
     * @param deckName: Name of the deck to add.
     */
    void addDeck(QString deckName);

    /**
     * @brief addDeckToTable: Adds a deck to the decks table at the specified row.
     * @param row: The row to add the deck to.
     * @param deck: The deck to add to the table.
     */
    void addDeckToTable(int row, SRFRS::Deck deck);

    /**
     * @brief addDeckToTable: Overloaded, creates a new row in the table and adds
     *                        the deck to that row.
     * @param deck: The deck to add to the table.
     */
    void addDeckToTable(SRFRS::Deck deck);

    /**
     * @brief addDeckButton: Adds a button linked to deckName in the specified row.
     * @param row: The row to add the button to.
     * @param deckName: The deck name to link the button to.
     */
    void addDeckButton(int row, QString deckName);

    /**
     * @brief getDeckRow: Returns the row containing the deck with the name deckName.
     * @param deckName: The name of the deck to find.
     * @return: The row containing the deck with the name deckName.
     */
    int getDeckRow(QString deckName);

    /**
     * @brief renameDeck: Renames the deck to deckName.
     * @param deckName: The new name of the deck.
     * @param deck: The deck to rename.
     */
    void renameDeck(QString deckName, QSharedPointer<SRFRS::Deck> deck);

    // flashcard methods

    /**
     * @brief addFlashcard: Adds a flashcard with specified ID, front, back and deck
     *                      to the system.
     * @param id: The ID of the flashcard being added.
     * @param front: The front side of the flashcard being added.
     * @param back: The back side of the flashcard being added.
     * @param deck: The deck the flashcard belongs to.
     */
    void addFlashcard(int id, QStringList front, QStringList back, QString deck);

    /**
     * @brief addFlashcardToDeck: Adds a flashcard to its associated deck's vector.
     * @param card: The flashcard to add to its deck.
     */
    void addFlashcardToDeck(QSharedPointer<SRFRS::Flashcard> card);

    /**
     * @brief addFlashcardToTable: Adds a flashcard to the flashcard table at
     *                             the specified row.
     * @param row: The row to add the flashcard to.
     * @param card: The card to add to the table.
     */
    void addFlashcardToTable(int row, SRFRS::Flashcard card);

    /**
     * @brief addFlashcardToTable: Creates a new row and add the flashcard to that row.
     * @param flashcard: The flashcard to add.
     */
    void addFlashcardToTable(SRFRS::Flashcard flashcard);

    /**
     * @brief addFlashcardButton: Adds a button linked to the ID in the specified row.
     * @param row: The row to add the button to.
     * @param id: The ID to link the button to.
     */
    void addFlashcardButton(int row, int id);

    /**
     * @brief getFlashcardRow: Returns the row containing the flashcard with specified id.
     * @param id: The ID to search for.
     * @return: The row containing the flashcard with specified id.
     */
    int getFlashcardRow(int id);

    /**
     * @brief resetFlashcardIDs: Iterate through all flashcards and reset their IDs.
     */
    void resetFlashcardIDs();

    /**
     * @brief deleteFlashcard: Removes the flashcard with specified ID from the system.
     * @param ID: The ID of the flashcard to remove.
     * @param card: The flashcard to delete.
     */
    void deleteFlashcard(int ID, QSharedPointer<SRFRS::Flashcard> card);

    // review methods

    /**
     * @brief addDeckToReview: Adds the specified deck to the review table in the specified row.
     * @param row: The row to add the deck to.
     * @param deck: The deck to add.
     */
    void addDeckToReview(int row, SRFRS::Deck deck);

    /**
     * @brief addDeckToReview: Creates a new row and adds the deck to that row.
     * @param deck: The deck to add.
     */
    void addDeckToReview(SRFRS::Deck deck);

    /**
     * @brief addReviewButton: Adds a button linked to deckName in the specified row.
     * @param row: The row to add the button in.
     * @param deckName: The deck name to link the button to.
     */
    void addReviewButton(int row, QString deckName);

    /**
     * @brief getReviewRow: Returns the row containing the deck with the specified deck name.
     * @param deckName: The deck name to search for.
     * @return: The row containing the deck with the specified deck name.
     */
    int getReviewRow(QString deckName);

    /**
     * @brief updateFlashcardTable: Iterate through all flashcards and update the flashcards
     *                              table with any changes.
     */
    void updateFlashcardTable();

    /**
     * @brief updateDeckTable: Iterate through all decks and update the decks table
     *                         with any changes to.
     */
    void updateDeckTable();

    /**
     * @brief updateReviewTable: Check if there are any decks to add to the review table,
     *                           update the review table with any changes.
     */
    void updateReviewTable();

    /**
     * @brief moveToLogin: Open the login window, delete this window.
     */
    void moveToLogin();

    /**
     * @brief logout: Log out the current user.
     */
    void logout();

private slots:
    void on_btnLogout_clicked();

    void on_btnSettings_clicked();

    void on_createFlashcard_clicked();

    void on_createDeck_clicked();

    void deck_rename(QString deckName);

    void deck_edit(QString deckName);

    void deck_delete(QString deckName);

    void deck_review(QString deckName);

    void on_decksTable_cellDoubleClicked(int row, int column);

    void flashcard_preview(int ID);

    void flashcard_edit(int ID);

    void flashcard_delete(int ID);

    void on_flashcardsTable_cellDoubleClicked(int row, int column);

    void on_reviewTable_cellDoubleClicked(int row, int column);

private:

    void mainInitUI();
    void init();

    Ui::MainWindow *ui;

    LoginWindow *_loginWindow;

    QString _username;
    QString _dirPath;

    SRFRS::AccountManager _accountManager;
    SRFRS::DeckManager _deckManager;
    SRFRS::FlashcardManager _flashcardManager;

    QStringList _decksToReview;
};

#endif // MAINWINDOW_H
