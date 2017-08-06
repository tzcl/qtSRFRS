#include "settings.h"
#include "ui_settings.h"

#include <QMessageBox>

Settings::Settings(MainWindow *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    parent(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    setWindowTitle("SRFRS");
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_btn_delete_account_clicked()
{
    if (QMessageBox::Yes == QMessageBox(QMessageBox::Critical, "SRFRS", "Are you sure you want to delete your account?\nThis cannot be undone.", QMessageBox::Yes|QMessageBox::No).exec())
    {
        if(parent->getAccountManager().deleteUser(parent->getUsername())) {

            // close this settings dialog
            close();

            parent->moveToLogin();
        }
    }
}
