#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "accountmanager.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btn_login_clicked();

    void on_btn_logout_clicked();

    void on_btn_create_clicked();

    void on_btn_signin_clicked();

    void on_btn_register_clicked();

private:
    /**
     * @brief loginInitUI: initialise UI components for the login screen
     */
    void loginInitUI();

    /**
     * @brief mainInitUI: initialise UI components for the main screen
     */
    void mainInitUI();

    Ui::MainWindow *ui;

    SRFRS::AccountManager accountManager;
};

#endif // MAINWINDOW_H
