#include "deckeditor.h"
#include "ui_deckeditor.h"

#include <QString>

DeckEditor::DeckEditor(SRFRS::Deck &deck, QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::DeckEditor),
    _deck(deck)
{
    ui->setupUi(this);

    this->setWindowTitle("SRFRS");
}

DeckEditor::~DeckEditor()
{
    delete ui;
}

void DeckEditor::on_btn_rename_clicked()
{
    _deck.setName(ui->txt_name->text());
}

void DeckEditor::on_btn_delete_clicked()
{
    _deck.setName("!!remove!!");
}
