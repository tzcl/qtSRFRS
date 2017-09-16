#include "flashcardpreviewer.h"
#include "ui_flashcardpreviewer.h"

#include <QRegExp>
#include <QString>
#include <QPixmap>
#include <QBitmap>
#include <QDebug>
#include <QMovie>

FlashcardPreviewer::FlashcardPreviewer(QString dirPath, SRFRS::Flashcard flashcard, QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::FlashcardPreviewer),
    _show(false)
{
    ui->setupUi(this);

    setWindowTitle("SRFRS");

    // init button to "show"
    ui->btn_toggle->setText("Show");
    ui->btn_toggle->adjustSize();

    QString card_front = flashcard.getFront().join("\n");
    QString card_back = flashcard.getBack().join("\n");

    QRegExp imagePattern = QRegExp("\\[((.*)\\.(jpg|png|gif))\\]");

    imagePattern.indexIn(card_front);
    QString frontImage;
    imagePattern.cap(1) == "" ? frontImage = "text" : frontImage = dirPath + "/res/" + imagePattern.cap(1);
    if(imagePattern.cap(3) == "gif") frontImage = "gif";

    QPixmap frontPixmap(frontImage);

    if(frontImage == "text") {
        ui->lbl_front->setText(card_front);
        ui->lbl_front->adjustSize();
    } else if(frontImage == "gif") {
        QMovie *movie = new QMovie(dirPath + "/res/" + imagePattern.cap(1));
        ui->lbl_front->setMovie(movie);
        movie->start();
    } else {
        ui->lbl_front->setPixmap(frontPixmap);
        ui->lbl_front->setMask(frontPixmap.mask());
    }

    imagePattern.indexIn(card_back);
    QString backImage = "text";
    imagePattern.cap(1) == "" ? backImage = "text" : backImage = dirPath + "/res/" + imagePattern.cap(1);
    if(imagePattern.cap(3) == "gif") backImage = "gif";

    QPixmap backPixmap(backImage);

    if(backImage == "text") {
        ui->lbl_back->setText(card_back);
        ui->lbl_back->adjustSize();
    } else if(frontImage == "gif") {
        QMovie *movie = new QMovie(dirPath + "/res/" + imagePattern.cap(1));
        ui->lbl_front->setMovie(movie);
        movie->start();
    } else {
        ui->lbl_back->setPixmap(backPixmap);
        ui->lbl_back->setMask(backPixmap.mask());
    }

    ui->lbl_back->setVisible(_show);

    //
}

FlashcardPreviewer::~FlashcardPreviewer()
{
    delete ui;
}

void FlashcardPreviewer::on_btn_toggle_clicked()
{
    _show = !_show;

    ui->lbl_back->setVisible(_show);
    _show ? ui->btn_toggle->setText("Hide") : ui->btn_toggle->setText("Show");
}
