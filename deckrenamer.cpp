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

    ui->txt_name->setText(deck->getName());
    ui->txt_name->setFocus();
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
    _deck->setName(ui->txt_name->text());
}

void DeckRenamer::on_txt_name_textEdited(const QString &string)
{
    if(string != _deck->getName()) {
        ui->txt_name->setStyleSheet("background: yellow");
        ui->txt_name->setToolTip("Value changed");
    } else {
        ui->txt_name->setStyleSheet("");
        ui->txt_name->setToolTip("");
    }

    auto decks = getParent()->getDeckManager().getDeckNames();
    decks.removeAll(_deck->getName());

    if(decks.contains(string)) {
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
            ui->txt_name->setStyleSheet("background: red");
            ui->buttonBox->setToolTip("Deck name already taken");
            ui->txt_name->setToolTip("Deck name already taken");
        } else if(string == "") {
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
            ui->txt_name->setStyleSheet("background: red");
            ui->buttonBox->setToolTip("Deck name can't be empty");
            ui->txt_name->setToolTip("Deck name can't be empty");
        } else if(string.contains(";;")) {
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
            ui->txt_name->setStyleSheet("background: red");
            ui->buttonBox->setToolTip("Deck name can't contain ;;");
            ui->txt_name->setToolTip("Deck name can't contain ;;");
        } else {
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
            ui->buttonBox->setToolTip("");

            if(ui->txt_name->styleSheet() != "background: yellow") {
                ui->txt_name->setStyleSheet("");
                ui->txt_name->setToolTip("");
            }
        }
}
