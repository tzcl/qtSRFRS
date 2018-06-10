#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "accountmanager.h"
#include "settings.h"

#include "deckcreator.h"
#include "deckeditor.h"
#include "deckrenamer.h"
#include "deckreviewer.h"

#include "flashcardcreator.h"
#include "flashcardeditor.h"
#include "flashcardpreviewer.h"

#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QRegularExpression>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>
#include <QSharedPointer>

MainWindow::MainWindow(QString username, QString dirPath, SRFRS::AccountManager accountManager, LoginWindow *loginWindow, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _loginWindow(loginWindow),
    _username(username),
    _dirPath(dirPath),
    _accountManager(accountManager),
    _deckManager(),
    _flashcardManager(),
    _decksToReview()
{
    ui->setupUi(this);

    // make window non-resizable
    this->setFixedSize(960, 720);
    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    // set icon
    this->setWindowIcon(QIcon(":/icons/logo.png"));

    // run init methods
    mainInitUI();
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mainInitUI()
{
    ui->btnSettings->setIcon(QIcon(":/icons/cogs.png"));
    ui->btnLogout->setIcon(QIcon(":/icons/power-off.png"));

    // set up review table

    // hide vertical headers
    ui->reviewTable->verticalHeader()->hide();

    // take care of resizing
    ui->reviewTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->reviewTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->reviewTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->reviewTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    ui->reviewTable->horizontalHeader()->resizeSection(2, 101);

    // set up deck table

    // hide vertical headers
    ui->decksTable->verticalHeader()->hide();

    // take care of resizing
    ui->decksTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->decksTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->decksTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->decksTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    // make sure due column has size of 101
    ui->decksTable->horizontalHeader()->resizeSection(2, 101);

    //set up flashcard table

    // hide vertical headers
    ui->flashcardsTable->verticalHeader()->hide();

    // take care of resizing
    ui->flashcardsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->flashcardsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->flashcardsTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->flashcardsTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->flashcardsTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    // give front column fixed size of 250
    ui->flashcardsTable->horizontalHeader()->resizeSection(1, 250);
}

void MainWindow::init()
{
    // set username label
    ui->lblUsername->setText("Hi, " + _username + "!");
    ui->lblUsername->adjustSize();
    ui->lblUsername->move(ui->btnLogout->x() + ui->btnLogout->width() - ui->lblUsername->width() - 8, 22);

    // set up no review label
    ui->lblNoReview->adjustSize();
    ui->lblNoReview->setVisible(false);

    // initialise deckManager
    if(_deckManager.init(_username, _dirPath)) {
        // add loaded decks to table
        for(int i = 0; i < _deckManager.getDecks().size(); ++i) {
            addDeckToTable(*_deckManager.getDecks()[i]);
        }
    } else {
        qDebug() << "error initialising _deckManager!!";
    }

    // initialise flashcardManager
    if(_flashcardManager.init(_username, _dirPath)) {
        for(int i = 0; i < _flashcardManager.getFlashcards().size(); ++i) {
            auto flashcard = _flashcardManager.getFlashcards()[i];

            // add loaded flashcards to table
            addFlashcardToTable(*flashcard);

            // also add flashcards to decks
            addFlashcardToDeck(flashcard);
        }
    } else {
        qDebug() << "error initialising _flashcardManager!!";
    }

    // update decks table
    updateDeckTable();

    // load flashcards to review
    for(int i = 0; i < _deckManager.getDecks().size(); ++i) {
        auto deck = _deckManager.getDecks()[i];
        if(!deck->getReview().isEmpty()) {
            addDeckToReview(*deck);
        }
    }

    // update review table
    updateReviewTable();

    // move to mainPage
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->setFocus();
}

void MainWindow::addDeckToReview(int row, SRFRS::Deck deck)
{
    ui->reviewTable->setSortingEnabled(false);

    // write items to table

    ui->reviewTable->setItem(row, 0, new QTableWidgetItem(deck.getName()));
    ui->reviewTable->setItem(row, 1, new QTableWidgetItem(QString::number(deck.getReview().size())));

    QDate date = deck.getDate();

    ui->reviewTable->setItem(row, 2, new QTableWidgetItem(date.toString("dd/MM/yyyy")));

    // check if deck date is earlier than today
    if(date < QDate::currentDate()) {
        // if overdue, make text colour red
        ui->reviewTable->item(row, 2)->setTextColor(Qt::red);
    } else {
        // else, reset text colour
        ui->reviewTable->item(row, 2)->setTextColor(Qt::black);
    }

    // add button linked to deck name
    addReviewButton(row, deck.getName());

    // add deck to list of decks to review
    _decksToReview.append(deck.getName());

    ui->reviewTable->setSortingEnabled(true);
}

void MainWindow::addDeckToReview(SRFRS::Deck deck)
{    
    // create a new row
    ui->reviewTable->setRowCount(ui->reviewTable->rowCount() + 1);
    int row = ui->reviewTable->rowCount() - 1;

    // insert deck into new row
    addDeckToReview(row, deck);
}

void MainWindow::addReviewButton(int row, QString name)
{
    // create tool button
    QToolButton *button = new QToolButton();

    // set button icon
    button->setIcon(QIcon(":/icons/tick.png"));
    button->setCursor(Qt::PointingHandCursor);

    // link button with deck name
    QSignalMapper *mapper = new QSignalMapper();

    connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(button, name);
    connect(mapper, SIGNAL(mapped(QString)), this, SLOT(deck_review(QString)));

    // insert into review table
    ui->reviewTable->setCellWidget(row, 3, button);
}

int MainWindow::getReviewRow(QString deckName)
{
    // init to invalid number for validation
    int row = -1;

    for(int i = 0; i < ui->reviewTable->rowCount(); ++i) {
        // find the row of the deck in review table with matching deck name
        if(ui->reviewTable->item(i, 0)->text() == deckName) {
            row = i;
        }
    }

    return row;
}

void MainWindow::deck_review(QString deckName)
{
    // get the deck
    auto deck = _deckManager.getDeck(deckName);

    // open the deck reviewer form
    DeckReviewer reviewer(_dirPath, _flashcardManager, deck, this);
    reviewer.exec();

    // update flashcard table
    updateFlashcardTable();

    // update review table
    updateReviewTable();

    // update decks table
    updateDeckTable();
}

void MainWindow::addDeck(QString deckName)
{
    // get deck
    auto deck = QSharedPointer<SRFRS::Deck>::create(deckName);

    // add to deckmanager
    _deckManager.addDeck(deck);

    // add to table
    addDeckToTable(*deck);
}

void MainWindow::addDeckToTable(int row, SRFRS::Deck deck)
{
    ui->decksTable->setSortingEnabled(false);

    // write items to table
    ui->decksTable->setItem(row, 0, new QTableWidgetItem(deck.getName()));
    ui->decksTable->setItem(row, 1, new QTableWidgetItem(deck.getFlashcards()));

    // get date
    QDate date = deck.getDate();

    // check if deck has any flashcards
    if(deck.getVector().isEmpty()) {
        // if deck is empty, make table due date empty
        ui->decksTable->setItem(row, 2, new QTableWidgetItem(""));
    } else {
        // else, set due date to deck date
        ui->decksTable->setItem(row, 2, new QTableWidgetItem(date.toString("dd/MM/yyyy")));
    }

    // check if date is earlier than today
    if(date < QDate::currentDate()) {
        // if date is overdue, set text colour to red
        ui->decksTable->item(row, 2)->setTextColor(Qt::red);
    } else {
        // else, reset text colour
        ui->decksTable->item(row, 2)->setTextColor(Qt::black);
    }

    // add deck button to table
    addDeckButton(row, deck.getName());

    ui->decksTable->setSortingEnabled(true);
}

void MainWindow::addDeckToTable(SRFRS::Deck deck)
{
    // create a new row
    ui->decksTable->setRowCount(ui->decksTable->rowCount() + 1);
    int row = ui->decksTable->rowCount() - 1;

    // insert deck into new row
    addDeckToTable(row, deck);
}

void MainWindow::addDeckButton(int row, QString name)
{
    // create new tool button
    QToolButton *button = new QToolButton();

    // create new menu
    QMenu *menu = new QMenu(this);

    // create menu actions
    QAction *action_rename = new QAction("Rename", this);
    QAction *action_edit = new QAction("Edit", this);
    QAction *action_delete = new QAction("Delete", this);

    // add actions to menu
    menu->addAction(action_rename);
    menu->addAction(action_edit);
    menu->addAction(action_delete);

    // set up button
    button->setIcon(QIcon(":/icons/cogs.png"));
    button->setCursor(Qt::PointingHandCursor);
    button->setMenu(menu);
    button->setPopupMode(QToolButton::InstantPopup);

    // link button with deck name
    QSignalMapper *mapper_rename = new QSignalMapper();
    QSignalMapper *mapper_edit = new QSignalMapper();
    QSignalMapper *mapper_delete = new QSignalMapper();

    connect(action_rename, SIGNAL(triggered()), mapper_rename, SLOT(map()));
    connect(action_edit, SIGNAL(triggered()), mapper_edit, SLOT(map()));
    connect(action_delete, SIGNAL(triggered()), mapper_delete, SLOT(map()));

    mapper_rename->setMapping(action_rename, name);
    mapper_edit->setMapping(action_edit, name);
    mapper_delete->setMapping(action_delete, name);

    connect(mapper_rename, SIGNAL(mapped(QString)), this, SLOT(deck_rename(QString)));
    connect(mapper_edit, SIGNAL(mapped(QString)), this, SLOT(deck_edit(QString)));
    connect(mapper_delete, SIGNAL(mapped(QString)), this, SLOT(deck_delete(QString)));

    // add button to deck table
    ui->decksTable->setCellWidget(row, 3, button);
}

int MainWindow::getDeckRow(QString deckName)
{
    // validation
    int row = -1;

    for(int i = 0; i < ui->decksTable->rowCount(); ++i) {
        // find row with deck with matching deck name
        if(ui->decksTable->item(i, 0)->text() == deckName) {
            row = i;
        }
    }

    return row;
}

void MainWindow::renameDeck(QString deckName, QSharedPointer<SRFRS::Deck> deck)
{
    // update deck manager
    _deckManager.renameDeck(deckName, deck->getName());

    // update decks table
    int row = getDeckRow(deckName);

    // only if valid row
    if(row != -1) {
        // write new name
        ui->decksTable->item(row, 0)->setText(deck->getName());
        // reset button
        addDeckButton(row, deck->getName());
    }

    // update flashcards
    for(int i = 0; i < deck->getVector().size(); ++i) {
        // write to file
        int id = deck->getVector()[i]->getID();
        deck->getVector()[i]->setDeck(deck->getName());
        _flashcardManager.update(id, 1, deck->getName());

        // update flashcards table
        int row = getFlashcardRow(id);
        if(row != -1) {
            ui->flashcardsTable->item(row, 2)->setText(deck->getName());
        }
    }
}

void MainWindow::deck_rename(QString deckName)
{
    // get deck
    auto deck = _deckManager.getDeck(deckName);

    // open deck renamer form
    DeckRenamer renamer(deck, this);
    renamer.exec();

    // rename deck
    renameDeck(deckName, deck);

    // update review table
    updateReviewTable();
}

void MainWindow::deck_edit(QString deckName)
{
    // get deck
    auto deck = _deckManager.getDeck(deckName);

    // open deck editor form
    DeckEditor de(deck, this);
    de.exec();

    // update decks table
    addDeckToTable(getDeckRow(deckName), *deck);

    // update flashcard table
    updateFlashcardTable();

    // update review table
    updateReviewTable();
}

void MainWindow::deck_delete(QString deckName)
{
    // warn user before deleting
    if (QMessageBox::Yes == QMessageBox(QMessageBox::Warning, "SRFRS", "Are you sure you want to delete your deck \"" + deckName + "\"?\nThis will also delete all the flashcards in your deck.", QMessageBox::Yes|QMessageBox::No, this).exec())
    {
        // remove flashcards
        auto deck = _deckManager.getDeck(deckName);

        // delete flashcards
        for(int i = 0; i < deck->getVector().size(); ++i) {
            int id = deck->getVector()[i]->getID();
            _flashcardManager.removeFlashcard(id);

            // update flashcards table
            ui->flashcardsTable->removeRow(getFlashcardRow(id));
        }

        // reset flashcard IDs
        resetFlashcardIDs();

        // remove deck
        _deckManager.removeDeck(deckName);

        // remove from decks table
        int deck_row = getDeckRow(deckName);
        ui->decksTable->removeRow(deck_row);

        // remove from review table
        int review_row = getReviewRow(deckName);
        ui->reviewTable->removeRow(review_row);
    }
}

void MainWindow::addFlashcard(int id, QStringList front, QStringList back, QString deck)
{
    auto card = QSharedPointer<SRFRS::Flashcard>::create(id, front, back, deck, QDate::currentDate(), 0, 1, 2.5f);

    // add to flashcard manager
    _flashcardManager.addFlashcard(card);

    // add to table
    addFlashcardToTable(*card);

    // add to deck
    addFlashcardToDeck(card);

    updateReviewTable();
}

void MainWindow::addFlashcardToDeck(QSharedPointer<SRFRS::Flashcard> card)
{
    // add flashcard to deck's flashcard vector
    auto deck = _deckManager.getDeck(card->getDeck());
    deck->addCard(card);

    // update deck table
    updateDeckTable();
}

void MainWindow::addFlashcardToTable(int row, SRFRS::Flashcard card)
{
    ui->flashcardsTable->setSortingEnabled(false);

    // write items to table
    ui->flashcardsTable->setItem(row, 0, new QTableWidgetItem(QString::number(card.getID())));
    ui->flashcardsTable->setItem(row, 1, new QTableWidgetItem(card.getFront().at(0)));
    ui->flashcardsTable->setItem(row, 2, new QTableWidgetItem(card.getDeck()));

    // get flashcard review date
    QDate date = card.getReviewDate();

    ui->flashcardsTable->setItem(row, 3, new QTableWidgetItem(date.toString("dd/MM/yyyy")));

    // check if review date is earlier than today
    if(date < QDate::currentDate()) {
        // if overdue, set text colour to red
        ui->flashcardsTable->item(row, 3)->setTextColor(Qt::red);
    } else {
        // else, reset text colour
        ui->flashcardsTable->item(row, 3)->setTextColor(Qt::black);
    }

    // add flashcard button to table
    addFlashcardButton(row, card.getID());

    ui->flashcardsTable->setSortingEnabled(true);
}

void MainWindow::addFlashcardToTable(SRFRS::Flashcard card)
{
    // create a new row
    ui->flashcardsTable->setRowCount(ui->flashcardsTable->rowCount() + 1);
    int row = ui->flashcardsTable->rowCount() - 1;

    // insert flashcard in new row
    addFlashcardToTable(row, card);
}

void MainWindow::addFlashcardButton(int row, int ID)
{
    // create new tool button
    QToolButton *button = new QToolButton();

    // create menu
    QMenu *menu = new QMenu(this);

    // create menu actions
    QAction *action_preview = new QAction("Preview", this);
    QAction *action_edit = new QAction("Edit", this);
    QAction *action_delete = new QAction("Delete", this);

    // add actions to menu
    menu->addAction(action_preview);
    menu->addAction(action_edit);
    menu->addAction(action_delete);

    // set up button
    button->setIcon(QIcon(":/icons/cogs.png"));
    button->setCursor(Qt::PointingHandCursor);
    button->setMenu(menu);
    button->setPopupMode(QToolButton::InstantPopup);

    // link button to ID
    QSignalMapper *mapper_preview = new QSignalMapper();
    QSignalMapper *mapper_edit = new QSignalMapper();
    QSignalMapper *mapper_delete = new QSignalMapper();

    connect(action_preview, SIGNAL(triggered()), mapper_preview, SLOT(map()));
    connect(action_edit, SIGNAL(triggered()), mapper_edit, SLOT(map()));
    connect(action_delete, SIGNAL(triggered()), mapper_delete, SLOT(map()));

    mapper_preview->setMapping(action_preview, ID);
    mapper_edit->setMapping(action_edit, ID);
    mapper_delete->setMapping(action_delete, ID);

    connect(mapper_preview, SIGNAL(mapped(int)), this, SLOT(flashcard_preview(int)));
    connect(mapper_edit, SIGNAL(mapped(int)), this, SLOT(flashcard_edit(int)));
    connect(mapper_delete, SIGNAL(mapped(int)), this, SLOT(flashcard_delete(int)));

    // add button to table
    ui->flashcardsTable->setCellWidget(row, 4, button);
}

int MainWindow::getFlashcardRow(int id)
{
    // init to invalid in case assignment doesn't occur
    int row = -1;

    for(int i = 0; i < ui->flashcardsTable->rowCount(); ++i) {
        // find the row with flashcard with matching ID
        if(ui->flashcardsTable->item(i, 0)->text() == QString::number(id)) {
            row = i;
        }
    }

    return row;
}

void MainWindow::resetFlashcardIDs()
{
    for(int i = 0; i < _flashcardManager.getFlashcards().size(); ++i) {
        // get row for flashcard at index i
        int row = getFlashcardRow(_flashcardManager.getFlashcards()[i]->getID());

        // get the flashcards original index
        int oldId = ui->flashcardsTable->item(row, 0)->text().toInt();

        // set new ID to flashcard
        _flashcardManager.setID(i, oldId, i);

        // update ID in flashcard table
        ui->flashcardsTable->setItem(row, 0, new QTableWidgetItem(QString::number(i)));

        // reset flashcard button for row
        addFlashcardButton(row, i);
    }
}

void MainWindow::flashcard_preview(int ID)
{
    // open flashcard previewer form
    FlashcardPreviewer preview(_dirPath, *_flashcardManager.getFlashcard(ID), this);
    preview.exec();
}

void MainWindow::flashcard_edit(int ID)
{
    FlashcardEditor editor(_dirPath, _flashcardManager.getFlashcard(ID), _deckManager.getDeckNames(), this);
    editor.exec();

    // update flashcards table
    addFlashcardToTable(getFlashcardRow(ID), *_flashcardManager.getFlashcard(ID));

    // update deck table
    updateDeckTable();

    // update review table
    updateReviewTable();
}

void MainWindow::deleteFlashcard(int ID, QSharedPointer<SRFRS::Flashcard> card)
{
    //  remove from deck
    auto deck = _deckManager.getDeck(card->getDeck());
    deck->removeCard(card);

    // update decks table
    updateDeckTable();

    // remove flashcard
    _flashcardManager.removeFlashcard(ID);

    // update flashcards table
    int row = getFlashcardRow(ID);
    ui->flashcardsTable->removeRow(row);

    // reset IDs
    resetFlashcardIDs();

    // update review table
    updateReviewTable();
}

void MainWindow::flashcard_delete(int ID)
{
    // get flashcard
    auto card = _flashcardManager.getFlashcard(ID);

    // warn user before deleting
    if (QMessageBox::Yes == QMessageBox(QMessageBox::Warning, "SRFRS", "Are you sure you want to delete your flashcard (ID: " + QString::number(ID) + ")?\nThis will remove it from your deck, \"" + card->getDeck() + "\".", QMessageBox::Yes|QMessageBox::No, this).exec())
    {
        // delete the flashcard
        deleteFlashcard(ID, card);
    }
}

void MainWindow::updateFlashcardTable()
{
    for(int i = 0; i < ui->flashcardsTable->rowCount(); ++i) {
        // get id
        int id = ui->flashcardsTable->item(i, 0)->text().toInt();

        // add flashcard to flashcard table
        addFlashcardToTable(i, *_flashcardManager.getFlashcard(id));
    }
}

void MainWindow::updateDeckTable()
{
    for(int i = 0; i < ui->decksTable->rowCount(); ++i) {
        // get deck name
        QString deckName = ui->decksTable->item(i, 0)->text();

        // add deck to deck table
        addDeckToTable(i, *_deckManager.getDeck(deckName));
    }
}

void MainWindow::updateReviewTable()
{
    // check for decks to add
    for(int i = 0; i < _deckManager.getDecks().size(); ++i) {
        // get deck at index i
        auto deck = _deckManager.getDecks()[i];

        // check if deck needs to be reviewed
        if(!deck->getReview().isEmpty() && !_decksToReview.contains(deck->getName())) {
            // add deck to review
            addDeckToReview(*deck);
        }
    }

    // update table with deck data
    for(int i = 0; i < ui->reviewTable->rowCount(); ++i) {
        // get deck
        auto deck = _deckManager.getDeck(ui->reviewTable->item(i, 0)->text());

        // check if deck has any more flashcards to review
        if(!deck->getReview().isEmpty()) {
            // keep the deck in review table
            addDeckToReview(i, *deck);
        } else {
            // remove deck from review table
            ui->reviewTable->removeRow(i);

            // remove deck from the decks to review list
            _decksToReview.removeAll(deck->getName());
        }
    }

    // check if review table is empty
    if(ui->reviewTable->rowCount() == 0) {
        // show no review label
        ui->lblNoReview->setVisible(true);

        // hide review table
        ui->reviewTable->setVisible(false);
    } else {
        // hide no review label
        ui->lblNoReview->setVisible(false);

        // show review table
        ui->reviewTable->setVisible(true);
    }
}

void MainWindow::on_btnSettings_clicked()
{
    // open settings form
    Settings settings(this);
    settings.exec();
}

void MainWindow::on_createFlashcard_clicked()
{
    // get list of existing deck names
    QStringList deckNames = _deckManager.getDeckNames();

    // check if any decks exist
    if(deckNames.empty()) {
        // if no decks exist, tell user they have to create a deck
        QMessageBox::warning(this, "SRFRS", "You need a deck to add flashcards to :-(\nPlease click OK and create a deck.");

        // move to the decks tab
        ui->tabWidget->setCurrentIndex(1);

    } else {
        // open the flashcard creator form
        FlashcardCreator fc(_dirPath, deckNames, this);
        fc.exec();
    }
}

void MainWindow::on_createDeck_clicked()
{
    // open deck creator form
    DeckCreator dc(this);
    dc.exec();
}

void MainWindow::on_reviewTable_cellDoubleClicked(int row, int column)
{
    // get deck
    QString deckName = ui->reviewTable->item(row, 0)->text();

    // open deck reviewer form for the associated deck
    deck_review(deckName);
}

void MainWindow::on_decksTable_cellDoubleClicked(int row, int column)
{
    // get deck
    QString deckName = ui->decksTable->item(row, 0)->text();

    //open deck editor form for the associated deck
    deck_edit(deckName);
}

void MainWindow::on_flashcardsTable_cellDoubleClicked(int row, int column)
{
    // get ID
    int id = ui->flashcardsTable->item(row, 0)->text().toInt();

    // open flashcard previewer form for the associated flashcard
    flashcard_preview(id);
}

void MainWindow::moveToLogin()
{
    // open loginwindow
    _loginWindow->show();

    // delete this window
    deleteLater();
}

void MainWindow::logout()
{
    moveToLogin();
}

void MainWindow::on_btnLogout_clicked()
{
    // double check user wants to log out
    if (QMessageBox::Yes == QMessageBox(QMessageBox::Question, "SRFRS", "Are you sure you want to log out?", QMessageBox::Yes|QMessageBox::No, this).exec())
    {
        logout();
    }
}
