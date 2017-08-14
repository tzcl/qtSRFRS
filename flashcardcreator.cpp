#include "flashcardcreator.h"
#include "ui_flashcardcreator.h"

#include "flashcard.h"

// suppress question mark

FlashcardCreator::FlashcardCreator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FlashcardCreator)
{
    ui->setupUi(this);
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

    // add to deck
}
