#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "accountmanager.h"
#include "settings.h"

#include "deckcreator.h"
#include "deckeditor.h"
#include "deckrenamer.h"

#include "flashcardcreator.h"
#include "flashcardeditor.h"

#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>
#include <QSharedPointer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _username(""),
    _dirPath(QStandardPaths::writableLocation(QStandardPaths::DataLocation)),
    _accountManager(SRFRS::AccountManager(_dirPath))
{ 
    ui->setupUi(this);

    // make window non-resizable
    this->setFixedSize(960, 720);
    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    this->setWindowIcon(QIcon(":/icons/logo.png"));

    loginInitUI();

    mainInitUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loginInitUI()
{
    int centerX = this->rect().center().x();

    // center title
    int titleHeight = 50;
    ui->lbl_hello->move(centerX - ui->lbl_hello->width() / 2, titleHeight);

    // center welcome
    int welcomeHeight = 130;
    ui->lbl_welcome->move(centerX - ui->lbl_welcome->width() / 2, welcomeHeight);

    // center loginStacked
    int lsWidth = 360, lsHeight = 220, padding = 200;
    ui->loginStacked->setFixedSize(lsWidth, 300);
    ui->loginStacked->move(centerX - lsWidth / 2, lsHeight);

    // groupbox shadow
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(30);
    effect->setXOffset(0);
    effect->setYOffset(5);
    ui->loginStacked->setGraphicsEffect(effect);

    // setup first page of loginStacked
    int elementWidth = lsWidth - (padding / 2), elementHeight = 50;
    centerX = lsWidth / 2;  // update center to be relative to groupbox

    ui->txt_username->setFixedSize(elementWidth, elementHeight);
    ui->txt_password->setFixedSize(elementWidth, elementHeight);

    ui->txt_username->setPlaceholderText("username");
    ui->txt_password->setPlaceholderText("password");
    ui->txt_password->setEchoMode(QLineEdit::Password);

    ui->txt_username->move(centerX - elementWidth / 2, 30);
    ui->txt_password->move(centerX - elementWidth / 2, ui->txt_username->geometry().bottom() + 20);

    ui->btn_login->setFixedSize(elementWidth, elementHeight);
    ui->btn_login->move(centerX - elementWidth / 2, ui->txt_password->geometry().bottom() + 20);

    // position labels
    int totalWidth = ui->btn_create->width() + ui->lbl_create->width();
    ui->lbl_create->move(centerX - (totalWidth / 2), ui->btn_login->geometry().bottom() + 20);
    ui->btn_create->move(ui->lbl_create->geometry().right(), ui->lbl_create->geometry().top() - 2);

    // give username input focus
    ui->txt_username->setFocus();

    // setup second page of loginStacked

    // same elementWidth, height and centerX as before
    ui->txt_register_username->setFixedSize(elementWidth, elementHeight);
    ui->txt_register_password->setFixedSize(elementWidth, elementHeight);

    ui->txt_register_username->setPlaceholderText("username");
    ui->txt_register_password->setPlaceholderText("password");
    ui->txt_register_password->setEchoMode(QLineEdit::Password);

    ui->txt_register_username->move(centerX - elementWidth / 2, 30);
    ui->txt_register_password->move(centerX - elementWidth / 2, ui->txt_register_username->geometry().bottom() + 20);

    ui->btn_register->setFixedSize(elementWidth, elementHeight);
    ui->btn_register->move(centerX - elementWidth / 2, ui->txt_register_password->geometry().bottom() + 20);

    // position labels
    totalWidth = ui->btn_signin->width() + ui->lbl_signin->width();
    ui->lbl_signin->move(centerX - (totalWidth / 2), ui->btn_register->geometry().bottom() + 20);
    ui->btn_signin->move(ui->lbl_signin->geometry().right(), ui->lbl_signin->geometry().top() - 2);
}

void MainWindow::mainInitUI()
{
    ui->btn_settings->setIcon(QIcon(":/icons/cogs.png"));
    ui->btn_logout->setIcon(QIcon(":/icons/power-off.png"));

    // set up review table

    // hide vertical headers
    ui->review_table->verticalHeader()->hide();

    // take care of resizing
    ui->review_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->review_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->review_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    // set up deck table

    // hide vertical headers
    ui->decks_table->verticalHeader()->hide();

    // take care of resizing
    ui->decks_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->decks_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->decks_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->decks_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    //set up flashcard table

    // hide vertical headers
    ui->flashcards_table->verticalHeader()->hide();

    // take care of resizing
    ui->flashcards_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->flashcards_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->flashcards_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->flashcards_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->flashcards_table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    // give front column fixed size of 100
    ui->flashcards_table->horizontalHeader()->resizeSection(1, 250);
}

void MainWindow::toggleButtonState(QPushButton *button, bool state) {
    button->setEnabled(state);
}

void MainWindow::moveToLogin() {
    ui->txt_username->clear();
    ui->txt_password->clear();
    ui->txt_username->setFocus();

    ui->stackedWidget->setCurrentIndex(0);
    ui->loginStacked->setCurrentIndex(0);
    ui->btn_login->setEnabled(false);
}

void MainWindow::moveToRegister() {
    ui->txt_register_username->clear();
    ui->txt_register_password->clear();
    ui->txt_register_username->setFocus();

    ui->stackedWidget->setCurrentIndex(0);
    ui->loginStacked->setCurrentIndex(1);
    ui->btn_register->setEnabled(false);
}

void MainWindow::login() {
    // handle login stuff here
    // e.g. loading from data base

    // set username label
    ui->lbl_username->setText("Hi, " + _username + "!");
    ui->lbl_username->adjustSize();
    ui->lbl_username->move(ui->btn_logout->x() + ui->btn_logout->width() - ui->lbl_username->width() - 8, 22);

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
        // add loaded flashcards to table
        for(int i = 0; i < _flashcardManager.getFlashcards().size(); ++i) {
            addFlashcardToTable(*_flashcardManager.getFlashcards()[i]);
        }
    } else {
        qDebug() << "error initialising _flashcardManager!!";
    }

    // move to mainPage
    ui->stackedWidget->setCurrentIndex(1);
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->setFocus();
}

void MainWindow::logout() {

    moveToLogin();
}

void MainWindow::addDeck(QString deckName)
{
    auto deck = QSharedPointer<SRFRS::Deck>::create(deckName, 0, QDate::currentDate());

    // add to deckmanager
    _deckManager.addDeck(deck);

    // add to table
    addDeckToTable(*deck);
}

void MainWindow::addDeckToTable(SRFRS::Deck deck)
{
    ui->decks_table->setSortingEnabled(false);

    ui->decks_table->setRowCount(ui->decks_table->rowCount() + 1);

    int row = ui->decks_table->rowCount() - 1;

    ui->decks_table->setItem(row, 0, new QTableWidgetItem(deck.getName()));
    ui->decks_table->setItem(row, 1, new QTableWidgetItem(deck.getFlashcards()));
    ui->decks_table->setItem(row, 2, new QTableWidgetItem(deck.getDate().toString("dd/MM/yyyy")));

    addDeckButton(row, deck.getName());

    ui->decks_table->setSortingEnabled(true);
}

void MainWindow::addDeckButton(int row, QString name)
{
    QToolButton *button = new QToolButton();
    QMenu *menu = new QMenu(this);

    QAction *action_rename = new QAction("Rename", this);
    QAction *action_edit = new QAction("Edit", this);
    QAction *action_delete = new QAction("Delete", this);

    menu->addAction(action_rename);
    menu->addAction(action_edit);
    menu->addAction(action_delete);

    button->setIcon(QIcon(":/icons/cogs.png"));
    button->setCursor(Qt::PointingHandCursor);
    button->setMenu(menu);
    button->setPopupMode(QToolButton::InstantPopup);

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

    ui->decks_table->setCellWidget(row, 3, button);
}

int MainWindow::getDeckRow(QString deckName)
{
    // init to invalid in case assignment doesn't occur
    int row = -1;

    for(int i = 0; i < ui->decks_table->rowCount(); ++i) {
        if(ui->decks_table->item(i, 0)->text() == deckName) {
            row = i;
        }
    }

    return row;
}

void MainWindow::deck_rename(QString deckName)
{
    // need to update flashcards

    auto deck = _deckManager.getDeck(deckName);

    DeckRenamer renamer(*deck, this);
    renamer.exec();

    // update deck manager
    _deckManager.renameDeck(deckName, deck->getName());

    // update decks table
    int row = getDeckRow(deckName);

    // only if valid row
    if(row != -1) {
        // write new name
        ui->decks_table->item(row, 0)->setText(deck->getName());
        // reset button
        addDeckButton(row, deck->getName());
    }
}

void MainWindow::deck_edit(QString deckName)
{
    DeckEditor de(deckName, this);
    de.exec();
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
            ui->flashcards_table->removeRow(getFlashcardRow(id));
        }

        // reset flashcard IDs
        resetFlashcardIDs();

        // remove deck
        _deckManager.removeDeck(deckName);

        // remove from decks table
        int row = getDeckRow(deckName);
        ui->decks_table->removeRow(row);
    }
}

void MainWindow::addFlashcard(int id, QString front, QString back, QString deck)
{
    auto card = QSharedPointer<SRFRS::Flashcard>::create(id, front, back, deck, QDate::currentDate());

    // add to flashcard manager
    _flashcardManager.addFlashcard(card);

    // add to table
    addFlashcardToTable(*card);

    // add to deck
    addFlashcardToDeck(id, deck);

    // write to file here? deck stuff
}

void MainWindow::addFlashcardToDeck(int id, QString deckName)
{
    // add flashcard to deck's flashcard vector
    auto deck = _deckManager.getDeck(deckName);

    // use id to identify which card to add
    deck->addCard(_flashcardManager.getFlashcard(id));

    // write to file
    _deckManager.update(deckName, 1, deck->getFlashcards());

    // update decks table
    for(int i = 0; i < ui->decks_table->rowCount(); ++i) {
        if(ui->decks_table->item(i, 0)->text() == deckName) {
            ui->decks_table->item(i, 1)->setText(deck->getFlashcards());
        }
    }
}

void MainWindow::addFlashcardToTable(SRFRS::Flashcard card)
{
    ui->flashcards_table->setSortingEnabled(false);

    ui->flashcards_table->setRowCount(ui->flashcards_table->rowCount() + 1);

    int row = ui->flashcards_table->rowCount() - 1;

    ui->flashcards_table->setItem(row, 0, new QTableWidgetItem(QString::number(card.getID())));
    ui->flashcards_table->setItem(row, 1, new QTableWidgetItem(card.getFront()));
    ui->flashcards_table->setItem(row, 2, new QTableWidgetItem(card.getDeck()));
    ui->flashcards_table->setItem(row, 3, new QTableWidgetItem(card.getDate().toString("dd/MM/yyyy")));

    addFlashcardButton(row, card.getID());

    ui->flashcards_table->setSortingEnabled(true);
}

void MainWindow::addFlashcardButton(int row, int ID)
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

    mapper_preview->setMapping(action_preview, ID);
    mapper_edit->setMapping(action_edit, ID);
    mapper_delete->setMapping(action_delete, ID);

    connect(mapper_preview, SIGNAL(mapped(int)), this, SLOT(flashcard_preview(int)));
    connect(mapper_edit, SIGNAL(mapped(int)), this, SLOT(flashcard_edit(int)));
    connect(mapper_delete, SIGNAL(mapped(int)), this, SLOT(flashcard_delete(int)));

    ui->flashcards_table->setCellWidget(row, 4, button);
}

int MainWindow::getFlashcardRow(int id)
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

void MainWindow::resetFlashcardIDs()
{
    for(int i = 0; i < _flashcardManager.getFlashcards().size(); ++i) {
        int row = getFlashcardRow(_flashcardManager.getFlashcards()[i]->getID());
        int oldId = ui->flashcards_table->item(row, 0)->text().toInt();

        _flashcardManager.setID(i, oldId, i);
        ui->flashcards_table->setItem(row, 0, new QTableWidgetItem(QString::number(i)));

        addFlashcardButton(row, i);
    }
}

void MainWindow::flashcard_preview(int ID)
{
    qDebug() << "previewing";
}

void MainWindow::flashcard_edit(int ID)
{
    qDebug() << "editing!!";
}

void MainWindow::flashcard_delete(int ID)
{
    auto card = _flashcardManager.getFlashcard(ID);

    // warn user before deleting
    if (QMessageBox::Yes == QMessageBox(QMessageBox::Warning, "SRFRS", "Are you sure you want to delete your flashcard (ID: " + QString::number(ID) + ")?\nThis will remove it from your deck, \"" + card->getDeck() + "\".", QMessageBox::Yes|QMessageBox::No, this).exec())
    {
        //  remove from deck
        auto deck = _deckManager.getDeck(card->getDeck());
        deck->removeCard(_flashcardManager.getFlashcard(ID));

        // write to file
        _deckManager.update(deck->getName(), 1, deck->getFlashcards());

        // update decks table
        for(int i = 0; i < ui->decks_table->rowCount(); ++i) {
            if(ui->decks_table->item(i, 0)->text() == deck->getName()) {
                ui->decks_table->item(i, 1)->setText(deck->getFlashcards());
            }
        }

        // remove flashcard
        _flashcardManager.removeFlashcard(ID);

        // update flashcards table
        int row = getFlashcardRow(ID);
        ui->flashcards_table->removeRow(row);

        // reset IDs
        resetFlashcardIDs();
    }
}

void MainWindow::on_btn_login_clicked()
{
    _username = ui->txt_username->text();
    QString password = ui->txt_password->text();

    // reset login page
    ui->txt_username->clear();
    ui->txt_password->clear();
    ui->txt_username->setFocus();
    ui->btn_login->setEnabled(false);

    // validate inputs, username or password cannot be empty
    if(_accountManager.validLogin(_username, password)) {

        login();

    } else {

        // login failed, tell user
        if (QMessageBox::Yes == QMessageBox(QMessageBox::Warning, "SRFRS", "Your username or password was wrong :-(\nDo you need to create an account?", QMessageBox::Yes|QMessageBox::No, this).exec())
        {
            moveToRegister();
        }
    }
}

void MainWindow::on_btn_logout_clicked()
{
    if (QMessageBox::Yes == QMessageBox(QMessageBox::Question, "SRFRS", "Are you sure you want to log out?", QMessageBox::Yes|QMessageBox::No, this).exec())
    {
        logout();
    }
}

void MainWindow::on_btn_create_clicked()
{
    moveToRegister();
}

void MainWindow::on_btn_signin_clicked()
{
    moveToLogin();
}

void MainWindow::on_btn_register_clicked()
{
    QString username = ui->txt_register_username->text();
    QString password = ui->txt_register_password->text();
    QRegularExpression invalidUsernameCharacters("([^A-Za-z0-9-_])");
    QRegularExpression invalidPasswordCharacters("([^A-Za-z0-9-_!@#$%^&*])");

    moveToRegister();

    // test to see whether username/password contains invalid characters
    if(username.contains(invalidUsernameCharacters) || password.contains(invalidPasswordCharacters)) {

        QMessageBox::warning(this, "SRFRS", "Your username or password cannot contain punctuation or spaces.");

        // check if either username or password are empty
    } else if(username == "" || password == "") {

        QMessageBox::warning(this, "SRFRS", "Your username or password cannot be empty.");

    } else {

        // see if registration successful
        if(_accountManager.registerUser(username, password)) {

            // registration successful, tell user
            QMessageBox::information(this, "SRFRS", "Your account was registered :-)\nTry logging in.");

            moveToLogin();

        } else {
            // registration failed, username must not be unique
            QMessageBox::warning(this, "SRFRS", "Your username is already taken :-(\nPlease use another one.");
        }
    }
}

// toggle login/register buttons if inputs are empty
void MainWindow::on_txt_username_textEdited(const QString &arg1)
{
    // enable button if neither textbox is empty
    bool on = arg1 != "" && ui->txt_password->text() != "";
    toggleButtonState(ui->btn_login, on);

    if(on) {
        // login when user presses enter
        connect(ui->txt_username, SIGNAL(returnPressed()), ui->btn_login, SIGNAL(clicked()), Qt::UniqueConnection);
    } else {
        // disable pressing enter when button is disabled
        disconnect(ui->txt_username, SIGNAL(returnPressed()), 0, 0);
    }
}

void MainWindow::on_txt_password_textEdited(const QString &arg1)
{
    // enable button if neither textbox is empty
    bool on = arg1 != "" && ui->txt_username->text() != "";
    toggleButtonState(ui->btn_login, on);

    if(on) {
        // login when user presses enter
        connect(ui->txt_password, SIGNAL(returnPressed()), ui->btn_login, SIGNAL(clicked()), Qt::UniqueConnection);
    } else {
        // disable pressing enter when button is disabled
        disconnect(ui->txt_password, SIGNAL(returnPressed()), 0, 0);
    }
}

void MainWindow::on_txt_register_username_textEdited(const QString &arg1)
{
    // enable button if neither textbox is empty
    bool on = arg1 != "" && ui->txt_register_password->text() != "";
    toggleButtonState(ui->btn_register, on);

    if(on) {
        // register when user presses enter
        connect(ui->txt_register_username, SIGNAL(returnPressed()), ui->btn_register, SIGNAL(clicked()));
    } else {
        // disable pressing enter when button is disabled
        disconnect(ui->txt_register_username, SIGNAL(returnPressed()), 0, 0);
    }
}

void MainWindow::on_txt_register_password_textEdited(const QString &arg1)
{
    // enable button if neither textbox is empty
    bool on = arg1 != "" && ui->txt_register_username->text() != "";
    toggleButtonState(ui->btn_register, on);

    if(on) {
        // register when user presses enter
        connect(ui->txt_register_password, SIGNAL(returnPressed()), ui->btn_register, SIGNAL(clicked()), Qt::UniqueConnection);
    } else {
        // disable pressing enter when button is disabled
        disconnect(ui->txt_register_password, SIGNAL(returnPressed()), 0, 0);
    }
}

void MainWindow::on_btn_settings_clicked()
{
    Settings settings(this);
    settings.exec();
}

void MainWindow::on_create_flashcard_clicked()
{
    QStringList deckNames = _deckManager.getDeckNames();
    if(deckNames.empty()) {
        QMessageBox::warning(this, "SRFRS", "You need a deck to add flashcards to :-(\nPlease click OK and create a deck.");
        ui->tabWidget->setCurrentIndex(1);
    } else {
        FlashcardCreator fc(deckNames, this);
        fc.exec();
    }
}

void MainWindow::on_create_deck_clicked()
{
    DeckCreator dc(this);
    dc.exec();
}

void MainWindow::on_decks_table_cellDoubleClicked(int row, int column)
{
    // TODO: change this (user testing --> familiarity)
    if(column == 0) {
        // rename
        deck_rename(ui->decks_table->item(row, 0)->text());
    } else if(column == 1) {
        // edit
        QString deckName = ui->decks_table->item(row, 0)->text();
        DeckEditor de(deckName, this);
        de.exec();
    }
}

void MainWindow::on_flashcards_table_cellDoubleClicked(int row, int column)
{
    // same as above: change depending on usability testing
    if(column == 1 || column == 2) {

    }
}
