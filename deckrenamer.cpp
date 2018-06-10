#include "deckrenamer.h"
#include "ui_deckrenamer.h"

#include <QDialogButtonBox>
#include <QPushButton>
#include <QDebug>

DeckRenamer::DeckRenamer(QSharedPointer<SRFRS::Deck> deck, QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint),
    ui(new Ui::DeckRenamer),
    _deck(deck)
{
    ui->setupUi(this);

    setWindowTitle("SRFRS");

    // set up txtName

    ui->txtName->setText(deck->getName());
    ui->txtName->setFocus();
}

DeckRenamer::~DeckRenamer()
{
    delete ui;
}

MainWindow* DeckRenamer::getParent() {
    return dynamic_cast<MainWindow*>(parent());
}

void DeckRenamer::on_buttonBox_accepted()
{
    // update deck name
    // must be valid otherwise OK button would be disabled
    _deck->setName(ui->txtName->text());
}

void DeckRenamer::on_txtName_textEdited(const QString &string)
{
    // validate deck name

    // check if the text in txtName is different to the deck's current name
    if(string != _deck->getName()) {

        // set text colour to blue
        ui->txtName->setStyleSheet("QLineEdit { color: blue }");
        // update tool tip
        ui->txtName->setToolTip("Value changed");

    } else {

        // reset tool tip and style sheet
        ui->txtName->setStyleSheet("");
        ui->txtName->setToolTip("");
    }

    // get deck names
    QStringList decks = getParent()->getDeckManager().getDeckNames();

    // remove the current deck name from deck names
    // so the deck name isn't compared against itself
    decks.removeAll(_deck->getName());

    // check if deck name is unique
    if(decks.contains(string)) {

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
