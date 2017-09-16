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

    ui->txt_name->setText(deck->getName());

    ui->btn_add_card->adjustSize();
    ui->lbl_name->adjustSize();
    ui->lbl_deck_cards->adjustSize();

    //set up flashcard table

    // hide vertical headers
    ui->flashcards_table->verticalHeader()->hide();

    // take care of resizing
    ui->flashcards_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->flashcards_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->flashcards_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->flashcards_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    // load deck flashcards into table
    for(int i = 0; i < deck->getVector().size(); ++i) {
        addFlashcardToTable(*deck->getVector()[i]);
    }

    // TODO: get flashcard table working
    // TODO: write changes to actual deck table, flashcard table
    // TODO: double clicking on table
}

DeckEditor::~DeckEditor()
{
    delete ui;
}

MainWindow* DeckEditor::getParent() {
    return dynamic_cast<MainWindow*>(parent());
}

void DeckEditor::on_txt_name_textEdited(const QString &string)
{
    if(string != _deck->getName()) {
        ui->txt_name->setStyleSheet("background: yellow");
        ui->txt_name->setToolTip("Value changed");
    } else {
        ui->txt_name->setStyleSheet("");
        ui->txt_name->setToolTip("");
    }

    QStringList decks = getParent()->getDeckManager().getDeckNames();
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

int DeckEditor::getFlashcardRow(int id)
{
    // init to invalid in case assignment doesn't occur
    int row = -1;

    for(int i = 0; i < ui->flashcards_table->rowCount(); ++i) {
        if(ui->flashcards_table->item(i, 0)->text() == QString::number(id)) {
            row = i;
        }
    }

    return row;
}

void DeckEditor::resetIDs()
{
    for(int i = 0; i < _deck->getVector().size(); ++i) {
        int id = _deck->getVector()[i]->getID();
        ui->flashcards_table->setItem(i, 0, new QTableWidgetItem(QString::number(id)));
        addFlashcardButton(i, id);
    }
}

void DeckEditor::addFlashcardToTable(int row, SRFRS::Flashcard card)
{
    ui->flashcards_table->setSortingEnabled(false);

    ui->flashcards_table->setItem(row, 0, new QTableWidgetItem(QString::number(card.getID())));
    ui->flashcards_table->setItem(row, 1, new QTableWidgetItem(card.getFront().at(0)));
    ui->flashcards_table->setItem(row, 2, new QTableWidgetItem(card.getDate().toString("dd/MM/yyyy")));

    addFlashcardButton(row, card.getID());

    ui->flashcards_table->setSortingEnabled(true);
}

void DeckEditor::addFlashcardToTable(SRFRS::Flashcard card)
{
    ui->flashcards_table->setRowCount(ui->flashcards_table->rowCount() + 1);
    int row = ui->flashcards_table->rowCount() - 1;
    addFlashcardToTable(row, card);
}

void DeckEditor::addFlashcardButton(int row, int id)
{
    QToolButton *button = new QToolButton();
    QMenu *menu = new QMenu(this);

    QAction *action_preview = new QAction("Preview", this);
    QAction *action_edit = new QAction("Edit", this);
    QAction *action_delete = new QAction("Delete", this);

    menu->addAction(action_preview);
    menu->addAction(action_edit);
    menu->addAction(action_delete);

    button->setIcon(QIcon(":/icons/cogs.png"));
    button->setCursor(Qt::PointingHandCursor);
    button->setMenu(menu);
    button->setPopupMode(QToolButton::InstantPopup);

    QSignalMapper *mapper_preview = new QSignalMapper();
    QSignalMapper *mapper_edit = new QSignalMapper();
    QSignalMapper *mapper_delete = new QSignalMapper();

    connect(action_preview, SIGNAL(triggered()), mapper_preview, SLOT(map()));
    connect(action_edit, SIGNAL(triggered()), mapper_edit, SLOT(map()));
    connect(action_delete, SIGNAL(triggered()), mapper_delete, SLOT(map()));

    mapper_preview->setMapping(action_preview, id);
    mapper_edit->setMapping(action_edit, id);
    mapper_delete->setMapping(action_delete, id);

    connect(mapper_preview, SIGNAL(mapped(int)), this, SLOT(flashcard_preview(int)));
    connect(mapper_edit, SIGNAL(mapped(int)), this, SLOT(flashcard_edit(int)));
    connect(mapper_delete, SIGNAL(mapped(int)), this, SLOT(flashcard_delete(int)));

    ui->flashcards_table->setCellWidget(row, 3, button);
}

void DeckEditor::flashcard_preview(int ID)
{
    FlashcardPreviewer previewer(getParent()->getPath(), *getParent()->getFlashcardManager().getFlashcard(ID), getParent());
    previewer.exec();
}

void DeckEditor::flashcard_edit(int ID)
{
    auto card = getParent()->getFlashcardManager().getFlashcard(ID);

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
    ui->flashcards_table->removeRow(row);

    resetIDs();
}

void DeckEditor::on_flashcards_table_cellDoubleClicked(int row, int column)
{
    int id = ui->flashcards_table->item(row, 0)->text().toInt();
    flashcard_edit(id);
}

void DeckEditor::on_btn_add_card_clicked()
{
    int before = _deck->getVector().size();

    FlashcardCreator fc(getParent()->getPath(), QStringList(_deck->getName()), getParent());
    fc.exec();

    // update table
    int after = _deck->getVector().size();
    if(before < after) {
        auto card = _deck->getVector()[after - 1];
        addFlashcardToTable(*card);
    }
}

void DeckEditor::on_buttonBox_accepted()
{
    // rename deck
    QString oldName = _deck->getName();
    QString newName = ui->txt_name->text();
    if(oldName != newName) {
        _deck->setName(newName);
        getParent()->renameDeck(oldName, _deck);
    }

    getParent()->updateTables();
}
