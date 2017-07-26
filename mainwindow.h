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
    void on_login_clicked();

    void on_logout_clicked();

private:
    Ui::MainWindow *ui;
    QString username;
};

#endif // MAINWINDOW_H
