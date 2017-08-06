#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(MainWindow *parent);
    ~Settings();

public slots:
    void on_btn_delete_account_clicked();

private:
    Ui::Settings *ui;
    MainWindow *parent;
};

#endif // SETTINGS_H
