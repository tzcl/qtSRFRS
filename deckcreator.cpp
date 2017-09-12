#include "deckcreator.h"
#include "ui_deckcreator.h"

#include <QDebug>
#include <QMessageBox>

DeckCreator::DeckCreator(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::DeckCreator)
{
    ui->setupUi(this);

    setWindowTitle("SRFRS");

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->txt_name->setStyleSheet("background: red");

    ui->txt_name->setFocus();
}

DeckCreator::~DeckCreator()
{
    delete ui;
}

MainWindow* DeckCreator::getParent() {
    return dynamic_cast<MainWindow*>(parent());
}

void DeckCreator::on_buttonBox_accepted()
{
    getParent()->addDeck(ui->txt_name->text());
}

void DeckCreator::on_txt_name_textEdited(const QString &string)
{
    if(getParent()->getDeckManager().getDeckNames().contains(string)) {
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
            ui->txt_name->setStyleSheet("background: red");
            ui->buttonBox->setToolTip("Deck name already taken");
        } else if(string == "") {
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
            ui->txt_name->setStyleSheet("background: red");
            ui->buttonBox->setToolTip("Deck name can't be empty");
        } else if(string.contains(";;")) {
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
            ui->txt_name->setStyleSheet("background: red");
            ui->buttonBox->setToolTip("Deck name can't contain ;;");
        } else {
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
            ui->txt_name->setStyleSheet("");
        }
}
