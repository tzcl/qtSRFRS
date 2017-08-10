#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "accountmanager.h"
#include "settings.h"

#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QRegExp>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    username(""),
    accountManager(SRFRS::AccountManager(1))
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

    // center logo
    ui->login_logo->setGeometry(0, 0, 150, 150);
    ui->login_logo->move(centerX - ui->login_logo->width()/ 2, 550);
    // hide logo
    ui->login_logo->setVisible(false);

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

    // register when user presses enter
    connect(ui->txt_register_username, SIGNAL(returnPressed()), ui->btn_register, SIGNAL(clicked()));
    connect(ui->txt_register_password, SIGNAL(returnPressed()), ui->btn_register, SIGNAL(clicked()));
}

void MainWindow::mainInitUI()
{
    ui->btn_settings->setIcon(QIcon(":/icons/cogs.png"));
    ui->btn_logout->setIcon(QIcon(":/icons/power-off.png"));
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

void MainWindow::logout() {
    // logout stuff here
    // write to database, move to login screen
}

SRFRS::AccountManager MainWindow::getAccountManager() {
    return accountManager;
}

QString MainWindow::getUser() {
    return username;
}

void MainWindow::on_btn_login_clicked()
{
    username = ui->txt_username->text();
    QString password = ui->txt_password->text();

    // reset login page
    ui->txt_username->clear();
    ui->txt_password->clear();
    ui->txt_username->setFocus();
    ui->btn_login->setEnabled(false);

    // validate inputs, username or password cannot be empty
    if(accountManager.validLogin(username, password)) {

        // move to mainPage
        ui->stackedWidget->setCurrentIndex(1);
        ui->tabWidget->setCurrentIndex(0);
        ui->tabWidget->setFocus();

        // set username label
        ui->lbl_username->setText(username);

    } else {

        // login failed, tell user
        if (QMessageBox::Yes == QMessageBox(QMessageBox::Warning, "SRFRS", "Your username or password was wrong :-(\nDo you need to create an account?", QMessageBox::Yes|QMessageBox::No).exec())
        {
            moveToRegister();
        }
    }
}

void MainWindow::on_btn_logout_clicked()
{
    if (QMessageBox::Yes == QMessageBox(QMessageBox::Question, "SRFRS", "Are you sure you want to log out?", QMessageBox::Yes|QMessageBox::No).exec())
    {
        logout();

        // move this into logout()
        moveToLogin();
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
    QRegExp invalidUsernameCharacters("([^A-Za-z0-9-_])");
    QRegExp invalidPasswordCharacters("([^A-Za-z0-9-_!@#$%^&*])");

    moveToRegister();

    // test to see whether username/password contains invalid characters
    if(username.contains(invalidUsernameCharacters) || password.contains(invalidPasswordCharacters)) {

        QMessageBox::warning(this, "SRFRS", "Your username or password cannot contain punctuation or spaces.");

        // check if either username or password are empty
    } else if(username == "" || password == "") {

        QMessageBox::warning(this, "SRFRS", "Your username or password cannot be empty.");

    } else {

        // see if registration successful
        if(accountManager.registerUser(username, password)) {

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
