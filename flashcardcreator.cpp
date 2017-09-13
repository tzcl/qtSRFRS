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

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->buttonBox->setToolTip("Flashcard can't be empty");
    ui->txt_front->setStyleSheet("background: red");
    ui->txt_back->setStyleSheet("background: red");
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
    QStringList front = ui->txt_front->toPlainText().split("\n");
    QStringList back = ui->txt_back->toPlainText().split("\n");

    getParent()->addFlashcard(getParent()->getFlashcardManager().getValidID(), front, back, ui->cb_decks->currentText());
}

bool FlashcardCreator::validText(QTextEdit *edit)
{
    // TODO: Validate flashcard doesn't already exist
    // TODO: Adding images to flashcards

    QString text = edit->toPlainText();

    if(text.contains(";;")) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->buttonBox->setToolTip("Flashcard can't contain ;;");
        edit->setStyleSheet("background: red");

        return false;
    } else if(text.isEmpty()) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->buttonBox->setToolTip("Flashcard can't be empty");
        edit->setStyleSheet("background: red");

        return false;
    } else {
        edit->setStyleSheet("");

        return true;
    }
}

void FlashcardCreator::validateInputs()
{
    bool frontValid = validText(ui->txt_front);
    bool backValid = validText(ui->txt_back);

    if(frontValid && backValid) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        ui->buttonBox->setToolTip("");
    }
}

void FlashcardCreator::on_txt_front_textChanged()
{
    validateInputs();
}

void FlashcardCreator::on_txt_back_textChanged()
{
    validateInputs();
}
