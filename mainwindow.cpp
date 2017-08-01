#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "accountmanager.h"
#include "settings.h"

#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QTimer>

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

    // position groupbox elements
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

    // position second page of loginStacked

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

void MainWindow::on_btn_login_clicked()
{
    // want to transition to main page
    QMessageBox::information(this, "Login Test", "Login successful? :-)");

        // set username label
        ui->lbl_username->setText(username);

    ui->lbl_username->setText(ui->txt_username->text());
    ui->tabWidget->setFocus();

    // clear lineEdits?
}

void MainWindow::on_btn_logout_clicked()
{
    if (QMessageBox::Yes == QMessageBox(QMessageBox::Question, "SRFRS", "Are you sure you want to log out?", QMessageBox::Yes|QMessageBox::No).exec())
    {
        // clean up log in form
        ui->txt_username->clear();
        ui->txt_password->clear();
        ui->txt_username->setFocus();

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

void MainWindow::on_btn_create_clicked()
{
    // clear inputs?
    ui->loginStacked->setCurrentIndex(1);
    ui->txt_register_username->setFocus();
}

void MainWindow::on_btn_signin_clicked()
{
    // clear inputs?
    ui->txt_username->setFocus();
    ui->loginStacked->setCurrentIndex(0);
}
