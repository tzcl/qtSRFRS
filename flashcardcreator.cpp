#include "flashcardcreator.h"
#include "ui_flashcardcreator.h"

#include "flashcard.h"

#include <QDebug>

FlashcardCreator::FlashcardCreator(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::FlashcardCreator)
{
    ui->setupUi(this);

    setWindowTitle("SRFRS");
    ui->lbl_front_text->setFocus();

    // disable tab as input, make it change text boxes
    ui->txt_front->setTabChangesFocus(true);
    ui->txt_back->setTabChangesFocus(true);
}

FlashcardCreator::~FlashcardCreator()
{
    delete ui;
}

void FlashcardCreator::on_buttonBox_accepted()
{
    // save front/back changes
    // validate input
    SRFRS::Flashcard newCard(ui->txt_front->toPlainText(), ui->txt_back->toPlainText());

    qDebug() << newCard.getFront() << ", " << newCard.getBack();
    // add to deck
}
