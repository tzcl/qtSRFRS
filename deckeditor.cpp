#include "deckeditor.h"
#include "ui_deckeditor.h"

#include <QString>
#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>
#include <QMessageBox>
#include <QDebug>

#include "flashcardcreator.h"
#include "flashcardeditor.h"
#include "flashcardpreviewer.h"

DeckEditor::DeckEditor(QSharedPointer<SRFRS::Deck> deck, QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint),
    ui(new Ui::DeckEditor),
    _deck(deck)
{
    ui->setupUi(this);

    this->setWindowTitle("SRFRS");

    // set up UI components

    ui->txtName->setText(deck->getName());

    ui->lblName->adjustSize();
    ui->lblDeckCards->adjustSize();

    //set up flashcard table

    // hide vertical headers
    ui->flashcardsTable->verticalHeader()->hide();

    // take care of resizing
    ui->flashcardsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->flashcardsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->flashcardsTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->flashcardsTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    // load deck flashcards into table
    for(int i = 0; i < deck->getVector().size(); ++i) {
        addFlashcardToTable(*deck->getVector()[i]);
    }
}

DeckEditor::~DeckEditor()
{
    delete ui;
}

MainWindow* DeckEditor::getParent() {
    return dynamic_cast<MainWindow*>(parent());
}

void DeckEditor::on_txtName_textEdited(const QString &string)
{
    // validate deck name

    // check if the text in txtName is different to the deck's current name
    if(string != _deck->getName()) {

        // set text colour to blue
        ui->txtName->setStyleSheet("QLineEdit { color: blue }");
        // update tool tip
        ui->txtName->setToolTip("Value changed");

    } else {

        // reset
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

int DeckEditor::getFlashcardRow(int id)
{
    // init to invalid in case assignment doesn't occur
    int row = -1;

    for(int i = 0; i < ui->flashcardsTable->rowCount(); ++i) {
        // find the row with matching ID
        if(ui->flashcardsTable->item(i, 0)->text() == QString::number(id)) {
            row = i;
        }
    }

    return row;
}

void DeckEditor::resetIDs()
{
    for(int i = 0; i < _deck->getVector().size(); ++i) {
        // get flashcard ID
        int id = _deck->getVector()[i]->getID();

        // update flashcard table with new ID
        ui->flashcardsTable->setItem(i, 0, new QTableWidgetItem(QString::number(id)));

        // link flashcard button to new ID
        addFlashcardButton(i, id);
    }
}

void DeckEditor::addFlashcardToTable(int row, SRFRS::Flashcard card)
{
    ui->flashcardsTable->setSortingEnabled(false);

    ui->flashcardsTable->setItem(row, 0, new QTableWidgetItem(QString::number(card.getID())));
    ui->flashcardsTable->setItem(row, 1, new QTableWidgetItem(card.getFront().at(0)));
    ui->flashcardsTable->setItem(row, 2, new QTableWidgetItem(card.getReviewDate().toString("dd/MM/yyyy")));

    addFlashcardButton(row, card.getID());

    ui->flashcardsTable->setSortingEnabled(true);
}

void DeckEditor::addFlashcardToTable(SRFRS::Flashcard card)
{
    ui->flashcardsTable->setRowCount(ui->flashcardsTable->rowCount() + 1);

    // add a new row
    int row = ui->flashcardsTable->rowCount() - 1;

    // insert flashcard in new row
    addFlashcardToTable(row, card);
}

void DeckEditor::addFlashcardButton(int row, int id)
{
    // create new tool button
    QToolButton *button = new QToolButton();

    // create new menu
    QMenu *menu = new QMenu(this);

    // create actions
    QAction *action_preview = new QAction("Preview", this);
    QAction *action_edit = new QAction("Edit", this);
    QAction *action_delete = new QAction("Delete", this);

    // add actions to menu
    menu->addAction(action_preview);
    menu->addAction(action_edit);
    menu->addAction(action_delete);

    // set up tool button
    button->setIcon(QIcon(":/icons/cogs.png"));
    button->setCursor(Qt::PointingHandCursor);

    // add menu to tool button
    button->setMenu(menu);
    button->setPopupMode(QToolButton::InstantPopup);

    // set up mapping
    QSignalMapper *mapper_preview = new QSignalMapper();
    QSignalMapper *mapper_edit = new QSignalMapper();
    QSignalMapper *mapper_delete = new QSignalMapper();

    // connect interaction with menu actions with flashcard ID
    connect(action_preview, SIGNAL(triggered()), mapper_preview, SLOT(map()));
    connect(action_edit, SIGNAL(triggered()), mapper_edit, SLOT(map()));
    connect(action_delete, SIGNAL(triggered()), mapper_delete, SLOT(map()));

    mapper_preview->setMapping(action_preview, id);
    mapper_edit->setMapping(action_edit, id);
    mapper_delete->setMapping(action_delete, id);

    connect(mapper_preview, SIGNAL(mapped(int)), this, SLOT(flashcard_preview(int)));
    connect(mapper_edit, SIGNAL(mapped(int)), this, SLOT(flashcard_edit(int)));
    connect(mapper_delete, SIGNAL(mapped(int)), this, SLOT(flashcard_delete(int)));

    // add button to flashcard table
    ui->flashcardsTable->setCellWidget(row, 3, button);
}

void DeckEditor::flashcard_preview(int ID)
{
    // open flashcard previewer form
    FlashcardPreviewer previewer(getParent()->getPath(), *getParent()->getFlashcardManager().getFlashcard(ID), getParent());
    previewer.exec();
}

void DeckEditor::flashcard_edit(int ID)
{
    // get flashcard with ID
    auto card = getParent()->getFlashcardManager().getFlashcard(ID);

    // open flashcard editor form
    FlashcardEditor editor(getParent()->getPath(), card, QStringList(_deck->getName()), getParent());
    editor.exec();

    // update flashcards table
    addFlashcardToTable(getFlashcardRow(ID), *card);
}

void DeckEditor::flashcard_delete(int ID)
{
    auto card = getParent()->getFlashcardManager().getFlashcard(ID);

    // warn user before deleting
    if (QMessageBox::Yes == QMessageBox(QMessageBox::Warning, "SRFRS", "Are you sure you want to delete your flashcard (ID: " + QString::number(ID) + ")?\nThis will remove it from your deck, \"" + card->getDeck() + "\".", QMessageBox::Yes|QMessageBox::No, this).exec())
    {
        getParent()->deleteFlashcard(ID, card);
    }

    // update table
    int row = getFlashcardRow(ID);
    ui->flashcardsTable->removeRow(row);

    // reset flashcard IDs
    resetIDs();
}

void DeckEditor::on_flashcardsTable_cellDoubleClicked(int row, int column)
{
    // get ID
    int id = ui->flashcardsTable->item(row, 0)->text().toInt();

    // open flashcard previewer for flashcard with ID
    flashcard_preview(id);
}

void DeckEditor::on_btnAddCard_clicked()
{
    // keep track of vector size
    int before = _deck->getVector().size();

    // open flashcard creator form
    FlashcardCreator fc(getParent()->getPath(), QStringList(_deck->getName()), getParent());
    fc.exec();

    // get vector size after
    int after = _deck->getVector().size();

    // validate a new card has been added to the deck
    if(before < after) {
        // get the flashcard (will be the most recent one in vector)
        auto card = _deck->getVector()[after - 1];

        // add flashcard to table
        addFlashcardToTable(*card);
    }
}

void DeckEditor::on_buttonBox_accepted()
{
    // keep track of old name
    QString oldName = _deck->getName();

    // get the new name
    // (must be valid because otherwise the OK button would be disabled)
    QString newName = ui->txtName->text();

    // check if new name is different to old name
    if(oldName != newName) {

        // update deck name
        _deck->setName(newName);
        getParent()->renameDeck(oldName, _deck);
    }
}
