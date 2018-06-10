#include "loginwindow.h"
#include "ui_loginwindow.h"

#include "mainwindow.h"

#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDebug>

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow),
    _dirPath(QStandardPaths::writableLocation(QStandardPaths::DataLocation)),
    _accountManager(SRFRS::AccountManager(_dirPath)),
    _username()
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    //setAttribute(Qt::WA_DeleteOnClose);
    setWindowIcon(QIcon(":/icons/logo.png"));
    setWindowTitle("SRFRS");

    loginInitUI();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::loginInitUI()
{
    int centerX = this->rect().center().x();

    // center title
    int titleHeight = 50;
    ui->lblHello->move(centerX - ui->lblHello->width() / 2, titleHeight);

    // center welcome
    int welcomeHeight = 130;
    ui->lblWelcome->move(centerX - ui->lblWelcome->width() / 2, welcomeHeight);

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

    ui->txtUsername->setFixedSize(elementWidth, elementHeight);
    ui->txtPassword->setFixedSize(elementWidth, elementHeight);

    ui->txtUsername->setPlaceholderText("username");
    ui->txtPassword->setPlaceholderText("password");
    ui->txtPassword->setEchoMode(QLineEdit::Password);

    ui->txtUsername->move(centerX - elementWidth / 2, 30);
    ui->txtPassword->move(centerX - elementWidth / 2, ui->txtUsername->geometry().bottom() + 20);

    ui->btnLogin->setFixedSize(elementWidth, elementHeight);
    ui->btnLogin->move(centerX - elementWidth / 2, ui->txtPassword->geometry().bottom() + 20);

    // position labels
    int totalWidth = ui->btnCreate->width() + ui->lblCreate->width();
    ui->lblCreate->move(centerX - (totalWidth / 2), ui->btnLogin->geometry().bottom() + 20);
    ui->btnCreate->move(ui->lblCreate->geometry().right(), ui->lblCreate->geometry().top() - 2);

    // give username input focus
    ui->txtUsername->setFocus();

    // setup second page of loginStacked

    // same elementWidth, height and centerX as before
    ui->txtRegisterUsername->setFixedSize(elementWidth, elementHeight);
    ui->txtRegisterPassword->setFixedSize(elementWidth, elementHeight);

    ui->txtRegisterUsername->setPlaceholderText("username");
    ui->txtRegisterPassword->setPlaceholderText("password");
    ui->txtRegisterPassword->setEchoMode(QLineEdit::Password);

    ui->txtRegisterUsername->move(centerX - elementWidth / 2, 30);
    ui->txtRegisterPassword->move(centerX - elementWidth / 2, ui->txtRegisterUsername->geometry().bottom() + 20);

    ui->btnRegister->setFixedSize(elementWidth, elementHeight);
    ui->btnRegister->move(centerX - elementWidth / 2, ui->txtRegisterPassword->geometry().bottom() + 20);

    // position labels
    totalWidth = ui->btnSignin->width() + ui->lblSignin->width();
    ui->lblSignin->move(centerX - (totalWidth / 2), ui->btnRegister->geometry().bottom() + 20);
    ui->btnSignin->move(ui->lblSignin->geometry().right(), ui->lblSignin->geometry().top() - 2);
}

void LoginWindow::toggleButtonState(QPushButton *button, bool state) {
    button->setEnabled(state);
}

void LoginWindow::moveToLogin() {
    // reset current page
    ui->txtUsername->clear();
    ui->txtPassword->clear();
    ui->txtUsername->setFocus();
    ui->btnLogin->setEnabled(false);

    // move to login page
    ui->loginStacked->setCurrentIndex(0);
}

void LoginWindow::moveToRegister() {
    // reset current page
    ui->txtRegisterUsername->clear();
    ui->txtRegisterPassword->clear();
    ui->txtRegisterUsername->setFocus();
    ui->btnRegister->setEnabled(false);

    // move to register page
    ui->loginStacked->setCurrentIndex(1);
}

void LoginWindow::login()
{
    // Create a new instance of main window
    MainWindow *mainWindow = new MainWindow(_username, _dirPath, _accountManager, this, 0);

    // show the instance
    mainWindow->show();

    // hide this window
    hide();
}

void LoginWindow::on_btnLogin_clicked()
{
    // get user and password from text boxes
    _username = ui->txtUsername->text();
    QString password = ui->txtPassword->text();

    // reset login page
    ui->txtUsername->clear();
    ui->txtPassword->clear();
    ui->txtUsername->setFocus();
    ui->btnLogin->setEnabled(false);

    // validate inputs, username or password cannot be empty
    if(_accountManager.validLogin(_username, password)) {

        login();

    } else {

        // login failed, tell user
        if (QMessageBox::Yes == QMessageBox(QMessageBox::Warning, "SRFRS", "Your username or password was wrong :-(\nDo you need to create an account?", QMessageBox::Yes|QMessageBox::No, this).exec())
        {
            // move to register to help them create an account
            moveToRegister();
        }
    }
}

void LoginWindow::on_btnCreate_clicked()
{
    // move to register page
    moveToRegister();
}

void LoginWindow::on_btnSignin_clicked()
{
    // move to the login page
    moveToLogin();
}

void LoginWindow::on_btnRegister_clicked()
{
    // patterns for invalid usernames and passwords
    QRegularExpression invalidUsernameCharacters("([^A-Za-z0-9-_])");
    QRegularExpression invalidPasswordCharacters("([^A-Za-z0-9-_!@#$%^&*])");

    // get username and password from text boxes
    QString username = ui->txtRegisterUsername->text();
    QString password = ui->txtRegisterPassword->text();

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

            // move to login page
            moveToLogin();

        } else {
            // registration failed, username must not be unique
            QMessageBox::warning(this, "SRFRS", "Your username is already taken :-(\nPlease use another one.");

            // reset current page
            moveToRegister();
        }
    }
}

// toggle login/register buttons if inputs are empty
void LoginWindow::on_txtUsername_textEdited(const QString &string)
{
    // enable button if neither textbox is empty
    bool on = string != "" && ui->txtPassword->text() != "";
    toggleButtonState(ui->btnLogin, on);

    if(on) {
        // login when user presses enter
        connect(ui->txtUsername, SIGNAL(returnPressed()), ui->btnLogin, SIGNAL(clicked()), Qt::UniqueConnection);
    } else {
        // disable pressing enter when button is disabled
        disconnect(ui->txtUsername, SIGNAL(returnPressed()), 0, 0);
    }
}

void LoginWindow::on_txtPassword_textEdited(const QString &string)
{
    // enable button if neither textbox is empty
    bool on = string != "" && ui->txtUsername->text() != "";
    toggleButtonState(ui->btnLogin, on);

    if(on) {
        // login when user presses enter
        connect(ui->txtPassword, SIGNAL(returnPressed()), ui->btnLogin, SIGNAL(clicked()), Qt::UniqueConnection);
    } else {
        // disable pressing enter when button is disabled
        disconnect(ui->txtPassword, SIGNAL(returnPressed()), 0, 0);
    }
}

void LoginWindow::on_txtRegisterUsername_textEdited(const QString &string)
{
    // enable button if neither textbox is empty
    bool on = string != "" && ui->txtRegisterPassword->text() != "";
    toggleButtonState(ui->btnRegister, on);

    if(on) {
        // register when user presses enter
        connect(ui->txtRegisterUsername, SIGNAL(returnPressed()), ui->btnRegister, SIGNAL(clicked()));
    } else {
        // disable pressing enter when button is disabled
        disconnect(ui->txtRegisterUsername, SIGNAL(returnPressed()), 0, 0);
    }
}

void LoginWindow::on_txtRegisterPassword_textEdited(const QString &string)
{
    // enable button if neither textbox is empty
    bool on = string != "" && ui->txtRegisterUsername->text() != "";
    toggleButtonState(ui->btnRegister, on);

    if(on) {
        // register when user presses enter
        connect(ui->txtRegisterPassword, SIGNAL(returnPressed()), ui->btnRegister, SIGNAL(clicked()), Qt::UniqueConnection);
    } else {
        // disable pressing enter when button is disabled
        disconnect(ui->txtRegisterPassword, SIGNAL(returnPressed()), 0, 0);
    }
}
