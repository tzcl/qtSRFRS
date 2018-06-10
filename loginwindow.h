#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include "accountmanager.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

    /**
     * @brief moveToLogin: Move to the login page.
     */
    void moveToLogin();

    /**
     * @brief moveToRegister: Move to the register page.
     */
    void moveToRegister();

    /**
     * @brief login: Open the main window for the current user, hide this window.
     */
    void login();

    /**
     * @brief toggleButtonState: Toggle the button state to the specified state.
     * @param button: The button whose state to toggle.
     * @param state: The state to toggle to.
     */
    void toggleButtonState(QPushButton *button, bool state);

private slots:

    void on_btnLogin_clicked();

    void on_btnCreate_clicked();

    void on_btnSignin_clicked();

    void on_btnRegister_clicked();

    void on_txtPassword_textEdited(const QString &arg1);

    void on_txtUsername_textEdited(const QString &arg1);

    void on_txtRegisterUsername_textEdited(const QString &arg1);

    void on_txtRegisterPassword_textEdited(const QString &arg1);

private:
    void loginInitUI();

    Ui::LoginWindow *ui;

    QString _dirPath;
    SRFRS::AccountManager _accountManager;
    QString _username;

};

#endif // LOGINWINDOW_H
