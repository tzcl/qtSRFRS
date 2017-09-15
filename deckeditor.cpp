#include "deckeditor.h"
#include "ui_deckeditor.h"

#include <QString>

DeckEditor::DeckEditor(QSharedPointer<SRFRS::Deck> deck, QVector<QSharedPointer<SRFRS::Flashcard>> flashcards, QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint),
    ui(new Ui::DeckEditor),
    _deck(deck),
    _flashcards(flashcards)
{
    ui->setupUi(this);

    this->setWindowTitle("SRFRS");

    ui->lbl_name->adjustSize();
    ui->txt_name->setText(deck->getName());

    ui->lbl_deck_cards->adjustSize();
}

DeckEditor::~DeckEditor()
{
    delete ui;
}

MainWindow* DeckEditor::getParent() {
    return dynamic_cast<MainWindow*>(parent());
}
