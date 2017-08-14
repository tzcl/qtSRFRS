#include "deckcreator.h"
#include "ui_deckcreator.h"

// suppress question mark

DeckCreator::DeckCreator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeckCreator)
{
    ui->setupUi(this);
}

DeckCreator::~DeckCreator()
{
    delete ui;
}
