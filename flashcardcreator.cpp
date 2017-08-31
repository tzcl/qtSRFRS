#include "flashcardcreator.h"
#include "ui_flashcardcreator.h"

#include "flashcard.h"

#include <QDebug>
#include <QDate>

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

    // give focus to front editor
    ui->txt_front->setFocus();
}

FlashcardCreator::~FlashcardCreator()
{
    delete ui;
}

MainWindow* FlashcardCreator::getParent() {
    return dynamic_cast<MainWindow*>(parent());
}

void FlashcardCreator::on_buttonBox_accepted()
{
    // save front/back changes
    // validate input

    // 0 index or 1 index?
    SRFRS::Flashcard newCard(getParent()->getFlashcardID(), ui->txt_front->toPlainText(), ui->txt_back->toPlainText(), QDate::currentDate());

    getParent()->addFlashcard(newCard);
}
