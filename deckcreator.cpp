#include "deckcreator.h"
#include "ui_deckcreator.h"

#include "deck.h"

#include <QDebug>

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

void DeckCreator::on_buttonBox_accepted()
{
    SRFRS::Deck deck(ui->txt_name->text());

    qDebug() << ui->txt_name->text();
}
