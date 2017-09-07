#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /**
     * @brief moveToLogin: Change loginStacked index to 0, reset UI components.
     */
    void moveToLogin();

    /**
     * @brief moveToRegister: Change loginStacked index to 1, reset UI components.
     */
    void moveToRegister();

    /**
     * @brief login: Login the user, loading flashcards and decks.
     */
    void login();

    /**
     * @brief logout: Logout the current user, writing any changes to the database
     *                and moving to the login screen.
     */
    void logout();

    /**
     * @brief getAccountManager: Returns the main window's account manager instance.
     * @return: AccountManager instance.
     */
    SRFRS::AccountManager getAccountManager() { return _accountManager; }

    SRFRS::DeckManager getDeckManager() { return _deckManager; }

    SRFRS::FlashcardManager getFlashcardManager() { return _flashcardManager; }

    /**
     * @brief getUsername: Returns the username of the current user.
     * @return: The current user's username as a string.
     */
    QString getUser() { return _username; }

    Ui::MainWindow* getUI() { return ui; }

    void addDeckToTable(const SRFRS::Deck &deck);

    void removeDeckFromTable(const int row);

    void addDeck(SRFRS::Deck &deck);

    void removeDeck(int row);

    int getDeckRow(QString deckName);

    void addDeckButton(int row, QString name);

    QStringList deckNames() { return _deckManager.deckNames(); }

    void addFlashcardToTable(const SRFRS::Flashcard &card);

    void addFlashcardToDeck(int id, QString deckName);

    void addFlashcardButton(int row, int ID);

    void addFlashcard(SRFRS::Flashcard &card);

    void removeFlashcard(int row);

    int getFlashcardRow(int ID);

    int getFlashcardID() { return _flashcardManager.getFlashcards().size(); }

    void resetFlashcardIDs();

private slots:
    void on_btn_login_clicked();

    void on_btn_logout_clicked();

    void on_btn_create_clicked();

    void on_btn_signin_clicked();

    void on_btn_register_clicked();

    void on_txt_password_textEdited(const QString &arg1);

    void on_txt_username_textEdited(const QString &arg1);

    void on_txt_register_username_textEdited(const QString &arg1);

    void on_txt_register_password_textEdited(const QString &arg1);

    void on_btn_settings_clicked();

    void on_create_flashcard_clicked();

    void on_create_deck_clicked();

    void deck_rename(QString deckName);

    void deck_edit(QString deckName);

    void deck_delete(QString deckName);

    void on_decks_table_cellDoubleClicked(int row, int column);

    void flashcard_preview(int ID);

    void flashcard_edit(int ID);

    void flashcard_delete(int ID);

    void on_flashcards_table_cellDoubleClicked(int row, int column);

private:
    /**
     * @brief loginInitUI: Initialise UI components for the login screen.
     */
    void loginInitUI();

    /**
     * @brief mainInitUI: Initialise UI components for the main screen.
     */
    void mainInitUI();

    /**
     * @brief toggleButtonState: Toggle the state of the given button to be enabled or disabled.
     * @param button: The button to toggle.
     * @param state: The state to toggle the button to.
     */
    void toggleButtonState(QPushButton *button, bool state);

    Ui::MainWindow *ui;

    QString _username;
    QString _dirPath;

    SRFRS::AccountManager _accountManager;
    SRFRS::DeckManager _deckManager;
    SRFRS::FlashcardManager _flashcardManager;
};

#endif // MAINWINDOW_H
