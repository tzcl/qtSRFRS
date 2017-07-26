#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{ 
    ui->setupUi(this);

    int centerX = this->rect().center().x();
    int offs = 0;

    // center title
    int titleHeight = 50 + offs;
    ui->title->move(centerX - ui->title->width() / 2, titleHeight);

    // center welcome
    int welcomeHeight = 150 + offs;
    ui->welcome->move(centerX - ui->welcome->width() / 2, welcomeHeight);

    // center lineEdits
    int inputHeight = 250 + offs, leWidth = 350, leHeight = 40;

    ui->username->setFixedWidth(leWidth);
    ui->password->setFixedWidth(leWidth);

    ui->username->setFixedHeight(leHeight);
    ui->password->setFixedHeight(leHeight);

    ui->username->setPlaceholderText("Username");
    ui->password->setPlaceholderText("Password");
    ui->password->setEchoMode(QLineEdit::Password);

    ui->username->move(centerX - leWidth / 2, inputHeight);
    ui->password->move(centerX - leWidth / 2, inputHeight + ui->username->height() + 15);

    // login when user presses enter
    connect(ui->username, SIGNAL(returnPressed()), ui->login, SIGNAL(clicked()));
    connect(ui->password, SIGNAL(returnPressed()), ui->login, SIGNAL(clicked()));

    // center login
    ui->login->setFixedSize(leWidth, 60);
    ui->login->move(centerX - leWidth / 2, ui->password->geometry().bottom() + 20);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_login_clicked()
{
    // want to transition to main page
    //QMessageBox::information(this, "Login Test", "Login successful? :-)");

    // hide login
//    ui->centralWidget->setStyleSheet("");
//    ui->title->hide();
//    ui->welcome->hide();
//    ui->username->hide();
//    ui->password->hide();
//    ui->login->hide();

    // change to mainPage
    ui->stackedWidget->setCurrentIndex(1);
    ui->tabWidget->setCurrentIndex(0);

    ui->lbl_username->setText(ui->username->text());
    ui->tabWidget->setFocus();
}

void MainWindow::on_logout_clicked()
{
    if (QMessageBox::Yes == QMessageBox(QMessageBox::Question, "SRFRS", "Do you want to log out?", QMessageBox::Yes|QMessageBox::No).exec())
    {
        // clean up log in form
        ui->username->clear();
        ui->password->clear();
        ui->username->setFocus();

        ui->stackedWidget->setCurrentIndex(0);
    }
}
