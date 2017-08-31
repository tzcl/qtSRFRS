#include "flashcardeditor.h"
#include "ui_flashcardeditor.h"

FlashcardEditor::FlashcardEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FlashcardEditor)
{
    ui->setupUi(this);
}

FlashcardEditor::~FlashcardEditor()
{
    delete ui;
}
