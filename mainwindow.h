#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private:
    void loginInitUI();
    void mainInitUI();

    Ui::MainWindow *ui;
    QString username;
};

#endif // MAINWINDOW_H
