#include "deckreviewer.h"
#include "ui_deckreviewer.h"

#include <QDebug>
#include <QPixmap>
#include <QBitmap>
#include <QMovie>

DeckReviewer::DeckReviewer(QString dirPath, SRFRS::FlashcardManager flashcardManager, QSharedPointer<SRFRS::Deck> deck, QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint),
    ui(new Ui::DeckReviewer),
    _dirPath(dirPath),
    _flashcardManager(flashcardManager),
    _deck(deck),
    _review(deck->getReview()),
    _index(0),
    _size(QString::number(deck->getReview().size())),
    _imagePattern(QRegExp("\\[((.*)\\.(jpg|png|gif))\\]"))
{
    ui->setupUi(this);

    setWindowTitle("SRFRS");

    // set up labels
    ui->lblDeck->setText(deck->getName());
    ui->lblDeck->adjustSize();

    ui->lblProgress->setText(QString::number(_index + 1) + "/" + _size);
    ui->lblProgress->adjustSize();
    ui->lblProgress->move(480 - ui->lblProgress->width() - 10, 10);

    ui->lblFinished->setText("Congratulations. You have finished reviewing this deck.");
    ui->lblFinished->setVisible(false);

    // hide difficulty buttons and close buttons
    showDifficultyButtons(false);
    ui->btnClose->setVisible(false);

    // set up flashcard
    handleFlashcard();
}

DeckReviewer::~DeckReviewer()
{
    delete ui;
}

void DeckReviewer::incrementIndex()
{
    // increment index
    _index++;

    // update progress label
    ui->lblProgress->setText(QString::number(_index + 1) + "/" + _size);
    ui->lblProgress->adjustSize();
    ui->lblProgress->move(480 - ui->lblProgress->width() - 10, 10);
}

void DeckReviewer::showDifficultyButtons(bool active)
{
    ui->btnEasy->setVisible(active);
    ui->btnPrettyEasy->setVisible(active);
    ui->btnGood->setVisible(active);
    ui->btnPrettyHard->setVisible(active);
    ui->btnHard->setVisible(active);
}

void DeckReviewer::handleReview(int difficulty)
{
    showDifficultyButtons(false);

    // write to file
    _flashcardManager.schedule(_review[_index]->getID(), difficulty);

    // remove flashcard from review vector
    _deck->removeReviewCard(_review[_index]);

    // reset for next card
    if(_index + 1 < _size.toInt()) {
        incrementIndex();

        ui->btnShow->setVisible(true);

        handleFlashcard();
    } else {
        // reviewed last card
        ui->btnClose->setVisible(true);

        ui->lblFront->setVisible(false);
        ui->lblBack->setVisible(false);
        ui->line->setVisible(false);

        ui->lblFinished->setVisible(true);
    }
}

void DeckReviewer::handleFlashcard()
{
    // get flashcard
    auto flashcard = _review[_index];

    // get flashcard front and back
    QString card_front = flashcard->getFront().join("\n");
    QString card_back = flashcard->getBack().join("\n");

    // check if flashcard contains images

    // check flashcard front for images

    _imagePattern.indexIn(card_front);

    // check front of flashcard

    _imagePattern.indexIn(card_front);
    QString frontImage;

    // check if the image pattern returns any matches
    if(_imagePattern.cap(1) == "") {
        // no matches, must be text
        frontImage = "text";
    } else {
        // set frontImage to the match
        frontImage = _dirPath + "/res/" + _imagePattern.cap(1);
    }

    // check if file format is gif
    if(_imagePattern.cap(3) == "gif") frontImage = "gif";

    if(frontImage == "text") {
        ui->lblFront->setText(card_front);
    } else if(frontImage == "gif") {
        QMovie *movie = new QMovie(_dirPath + "/res/" + _imagePattern.cap(1));
        ui->lblFront->setMovie(movie);
        movie->start();
    } else {
        QPixmap frontPixmap(frontImage);
        ui->lblFront->setPixmap(frontPixmap);
        ui->lblFront->setMask(frontPixmap.mask());
    }

    // check back of flashcard

    _imagePattern.indexIn(card_back);
    QString backImage = "text";

    // check if image pattern returns any matches
    if(_imagePattern.cap(1) == "") {
        // no match, must be text
        backImage = "text";
    } else {
        // set backImage to the match
        backImage = _dirPath + "/res/" + _imagePattern.cap(1);
    }

    // check if file format is gif
    if(_imagePattern.cap(3) == "gif") backImage = "gif";

    if(backImage == "text") {
        ui->lblBack->setText(card_back);
    } else if(backImage == "gif") {
        QMovie *movie = new QMovie(_dirPath + "/res/" + _imagePattern.cap(1));
        ui->lblBack->setMovie(movie);
        movie->start();
    } else {
        QPixmap backPixmap(backImage);
        ui->lblBack->setPixmap(backPixmap);
        ui->lblBack->setMask(backPixmap.mask());
    }

    // init flashcard back to be invisible
    ui->lblBack->setVisible(false);
}

void DeckReviewer::on_btnShow_clicked()
{
    // show flashcard back
    ui->lblBack->setVisible(true);

    // show difficulty butons
    showDifficultyButtons(true);

    // hide show button
    ui->btnShow->setVisible(false);
}

void DeckReviewer::on_btnEasy_clicked()
{
    handleReview(5);
}

void DeckReviewer::on_btnPrettyEasy_clicked()
{
    handleReview(4);
}

void DeckReviewer::on_btnGood_clicked()
{
    handleReview(3);
}

void DeckReviewer::on_btnPrettyHard_clicked()
{
    handleReview(2);
}

void DeckReviewer::on_btnHard_clicked()
{
    handleReview(1);
}

void DeckReviewer::on_btnClose_clicked()
{
    // close this form
    done(QDialog::Accepted);
}
