#include "deckcreator.h"
#include "ui_deckcreator.h"

#include "deck.h"

#include <QDebug>
#include <QDate>
#include <QMessageBox>

DeckCreator::DeckCreator(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::DeckCreator)
{
    ui->setupUi(this);

    setWindowTitle("SRFRS");
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
    SRFRS::Deck deck(ui->txt_name->text(), 0, QDate::currentDate());

    getParent()->addDeck(deck);
}

void DeckCreator::on_txt_name_textEdited(const QString &arg1)
{
    if(getParent()->deckNames().contains(ui->txt_name->text())) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->txt_name->setStyleSheet("background: red");
    } else {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        ui->txt_name->setStyleSheet("");
    }
}
