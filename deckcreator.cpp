#include "deckcreator.h"
#include "ui_deckcreator.h"

#include <QDebug>
#include <QMessageBox>

DeckCreator::DeckCreator(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint),
    ui(new Ui::DeckCreator)
{
    ui->setupUi(this);

    setWindowTitle("SRFRS");

    // set up validation

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->buttonBox->setToolTip("Deck name can't be empty");
    ui->txtName->setStyleSheet("QLineEdit { color: red }");

    ui->txtName->setFocus();
}

DeckCreator::~DeckCreator()
{
    delete ui;
}

MainWindow* DeckCreator::getParent() {
    return dynamic_cast<MainWindow*>(parent());
}

void DeckCreator::on_buttonBox_accepted()
{
    getParent()->addDeck(ui->txtName->text());
}

void DeckCreator::on_txtName_textEdited(const QString &string)
{
    // validate deck name

    // check if deck name is unique
    if(getParent()->getDeckManager().getDeckNames().contains(string)) {

        // disable OK button
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

        // change text colour to red
        ui->txtName->setStyleSheet("QLineEdit { color: red }");

        // update tooltips
        ui->buttonBox->setToolTip("Deck name already taken");
        ui->txtName->setToolTip("Deck name already taken");

    } else if(string == "") {               // check if deck name is empty

        // disable OK button
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

        // change text colour to red
        ui->txtName->setStyleSheet("QLineEdit { color: red }");

        // update tool tips
        ui->buttonBox->setToolTip("Deck name can't be empty");
        ui->txtName->setToolTip("Deck name can't be empty");

    } else if(string.contains(";;")) {      // check if deck name contains ";;"
                                            // (";;" used as delimiter in .decks file)

        // disable OK button
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

        // change text colour to red
        ui->txtName->setStyleSheet("QLineEdit { color: red }");

        // update tool tips
        ui->buttonBox->setToolTip("Deck name can't contain ;;");
        ui->txtName->setToolTip("Deck name can't contain ;;");
    } else {

        // deck name is valid

        // enable OK button
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);

        if(ui->txtName->styleSheet() != "QLineEdit { color: blue }") {
            // reset tooltips and text colour
            ui->buttonBox->setToolTip("");
            ui->txtName->setToolTip("");
            ui->txtName->setStyleSheet("");
        }
    }
}
