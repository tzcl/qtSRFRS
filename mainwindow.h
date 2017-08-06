#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "accountmanager.h"

#include <QMainWindow>
#include <QPushButton>

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
     * @brief getAccountManager: Returns the main window's account manager instance.
     * @return: AccountManager instance.
     */
    SRFRS::AccountManager getAccountManager();

    /**
     * @brief getUsername: Returns the username of the current user.
     * @return: The current user's username as a string.
     */
    QString getUsername();

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
    SRFRS::AccountManager accountManager;

};

#endif // MAINWINDOW_H
