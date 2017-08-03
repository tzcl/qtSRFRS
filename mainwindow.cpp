#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "accountmanager.h"

#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QRegExp>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    accountManager(SRFRS::AccountManager(1))
{ 
    ui->setupUi(this);

    // make window non-resizable
    this->setFixedSize(960, 720);
    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    loginInitUI();

    mainInitUI();
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

    // login when user presses enter
    connect(ui->txt_username, SIGNAL(returnPressed()), ui->btn_login, SIGNAL(clicked()));
    connect(ui->txt_password, SIGNAL(returnPressed()), ui->btn_login, SIGNAL(clicked()));

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

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_login_clicked()
{
    // add transition to main page?
    // clear lineEdits here?
    // QMessageBox::information(this, "Login Test", "Login successful? :-)");

    // validate inputs, username or password cannot be empty
    if(accountManager.validLogin(ui->txt_username->text(), ui->txt_password->text())) {

    } else {

        if (QMessageBox::Yes == QMessageBox(QMessageBox::Question, "SRFRS", "Your username or password was wrong :-(\nDo you need to create an account?", QMessageBox::Yes|QMessageBox::No).exec())
        {
            // change to create page

            // clear inputs
            ui->txt_username->clear();
            ui->txt_password->clear();

            // move to create page
            ui->loginStacked->setCurrentIndex(1);
            ui->txt_register_username->setFocus();

            return;

        } else {
            // do nothing
            return;
        }
    }


    // change to mainPage
    ui->stackedWidget->setCurrentIndex(1);
    ui->tabWidget->setCurrentIndex(0);

    ui->lbl_username->setText(ui->txt_username->text());
    ui->tabWidget->setFocus();
}

void MainWindow::on_btn_logout_clicked()
{
    if (QMessageBox::Yes == QMessageBox(QMessageBox::Question, "SRFRS", "Are you sure you want to log out?", QMessageBox::Yes|QMessageBox::No).exec())
    {
        // clean up log in form
        ui->txt_username->clear();
        ui->txt_password->clear();
        ui->txt_username->setFocus();

        ui->stackedWidget->setCurrentIndex(0);
    }
}

void MainWindow::on_btn_create_clicked()
{
    // clear inputs
    ui->txt_username->clear();
    ui->txt_password->clear();

    // move to create page
    ui->loginStacked->setCurrentIndex(1);
    ui->txt_register_username->setFocus();
}

void MainWindow::on_btn_signin_clicked()
{
    // clear inputs
    ui->txt_register_username->clear();
    ui->txt_register_password->clear();

    // move to login page
    ui->txt_username->setFocus();
    ui->loginStacked->setCurrentIndex(0);
}

void MainWindow::on_btn_register_clicked()
{
    QString username = ui->txt_register_username->text();
    QString password = ui->txt_register_password->text();
    QRegExp whitespace("\\s");

    // clear inputs
    ui->txt_register_username->clear();
    ui->txt_register_password->clear();
    ui->txt_register_username->setFocus();

    // test to see whether username/password has whitespace
    if(username.contains(whitespace) || password.contains(whitespace)) {

        QMessageBox::warning(this, "SRFRS", "Your username or password cannot have spaces.");

    } else if(username == "" || password == "") {

        // check if either username or password are empty
        QMessageBox::warning(this, "SRFRS", "Your username or password cannot be empty");

    } else {

        // see if registration successful
        if(accountManager.registerUser(username, password)) {

            // registration successful, tell user
            QMessageBox::information(this, "SRFRS", "Your account was registered :-)\nTry logging in");

            // move to login page
            ui->txt_username->setFocus();
            ui->loginStacked->setCurrentIndex(0);

        } else {

            // registration failed, username must not be unique
            QMessageBox::warning(this, "SRFRS", "Your username is already taken :-(\nPlease use another one");

        }
    }
}
