#include "settings.h"
#include "ui_settings.h"

#include <QMessageBox>
#include <QDialogButtonBox>

Settings::Settings(MainWindow *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    setWindowTitle("SRFRS");
}

Settings::~Settings()
{
    delete ui;
}

MainWindow* Settings::getParent() {
    return dynamic_cast<MainWindow*>(parent());
}

void Settings::on_btnDeleteAccount_clicked()
{
    // make sure user wants to delete their account
    if (QMessageBox::Yes == QMessageBox(QMessageBox::Critical, "SRFRS", "Are you sure you want to delete your account?\nThis cannot be undone.", QMessageBox::Yes|QMessageBox::No, this).exec())
    {
        // get accountManager instance from the main window
        // and delete the current user
        if(getParent()->getAccountManager().deleteUser(getParent()->getUser())) {

            // close this settings dialog
            close();

            // move to the login window
            getParent()->moveToLogin();
        }
    }
}
