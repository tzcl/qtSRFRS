#include "flashcardpreviewer.h"
#include "ui_flashcardpreviewer.h"

#include <QRegExp>
#include <QString>
#include <QPixmap>
#include <QBitmap>
#include <QDebug>
#include <QMovie>

FlashcardPreviewer::FlashcardPreviewer(QString dirPath, SRFRS::Flashcard flashcard, QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint),
    ui(new Ui::FlashcardPreviewer),
    _show(false)
{
    ui->setupUi(this);

    setWindowTitle("SRFRS");

    // init button to "show"
    ui->btnToggle->setText("Show");

    // set up flashcard front and back
    QString card_front = flashcard.getFront().join("\n");
    QString card_back = flashcard.getBack().join("\n");

    // check if flashcard contains images

    QRegExp imagePattern = QRegExp("\\[((.*)\\.(jpg|png|gif))\\]");

    // check front of flashcard

    imagePattern.indexIn(card_front);
    QString frontImage;

    // check if the image pattern returns any matches
    if(imagePattern.cap(1) == "") {
        // no matches, must be text
        frontImage = "text";
    } else {
        // set frontImage to the match
        frontImage = dirPath + "/res/" + imagePattern.cap(1);
    }

    // check if file format is gif
    if(imagePattern.cap(3) == "gif") frontImage = "gif";

    if(frontImage == "text") {
        ui->lblFront->setText(card_front);
    } else if(frontImage == "gif") {
        QMovie *movie = new QMovie(dirPath + "/res/" + imagePattern.cap(1));
        ui->lblFront->setMovie(movie);
        movie->start();
    } else {
        QPixmap frontPixmap(frontImage);
        ui->lblFront->setPixmap(frontPixmap);
        ui->lblFront->setMask(frontPixmap.mask());
    }

    // check back of flashcard

    imagePattern.indexIn(card_back);
    QString backImage = "text";

    // check if image pattern returns any matches
    if(imagePattern.cap(1) == "") {
        // no match, must be text
        backImage = "text";
    } else {
        // set backImage to the match
        backImage = dirPath + "/res/" + imagePattern.cap(1);
    }

    // check if file format is gif
    if(imagePattern.cap(3) == "gif") backImage = "gif";

    if(backImage == "text") {
        ui->lblBack->setText(card_back);
    } else if(backImage == "gif") {
        QMovie *movie = new QMovie(dirPath + "/res/" + imagePattern.cap(1));
        ui->lblBack->setMovie(movie);
        movie->start();
    } else {
        QPixmap backPixmap(backImage);
        ui->lblBack->setPixmap(backPixmap);
        ui->lblBack->setMask(backPixmap.mask());
    }

    // init flashcard back to be invisible
    ui->lblBack->setVisible(_show);
}

FlashcardPreviewer::~FlashcardPreviewer()
{
    delete ui;
}

void FlashcardPreviewer::on_btnToggle_clicked()
{
    // toggle show state
    _show = !_show;

    // set visibility state of flashcard back to _show
    ui->lblBack->setVisible(_show);

    // if _show, set button text to "hide", else set button text to "show"
    _show ? ui->btnToggle->setText("Hide") : ui->btnToggle->setText("Show");
}
