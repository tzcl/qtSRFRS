#include "deckeditor.h"
#include "ui_deckeditor.h"

#include <QString>

DeckEditor::DeckEditor(QString name, SRFRS::Deck &deck, QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::DeckEditor),
    _deck(deck)
{
    ui->setupUi(this);

    this->setWindowTitle("SRFRS | " + name);
}

DeckEditor::~DeckEditor()
{
    delete ui;
}
