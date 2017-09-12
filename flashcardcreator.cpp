#include "flashcardcreator.h"
#include "ui_flashcardcreator.h"

#include "flashcard.h"

#include <QDebug>
#include <QDate>

FlashcardCreator::FlashcardCreator(QStringList decks, QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::FlashcardCreator)
{
    ui->setupUi(this);

    setWindowTitle("SRFRS");

    ui->lbl_deck->adjustSize();

    ui->cb_decks->insertItems(0, decks);

    // disable tab as input, make it change text boxes
    ui->txt_front->setTabChangesFocus(true);
    ui->txt_back->setTabChangesFocus(true);

    // give focus to deck chooser
    ui->cb_decks->setFocus();

    // TODO: disable ok button until validated inputs
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

    // TODO: validate input, NO ;; allowed
    QString front = ui->txt_front->toPlainText();
    QString back = ui->txt_back->toPlainText();

    // write front, back to separate files

    // 0 index or 1 index?
    getParent()->addFlashcard(getParent()->getFlashcardManager().getValidID(), front, back, ui->cb_decks->currentText());
}
