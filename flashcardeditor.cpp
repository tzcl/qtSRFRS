#include "flashcardeditor.h"
#include "ui_flashcardeditor.h"

#include <QRegExp>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QDate>

FlashcardEditor::FlashcardEditor(QString dir, QSharedPointer<SRFRS::Flashcard> flashcard, QStringList decks, QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint),
    ui(new Ui::FlashcardEditor),
    _flashcard(flashcard),
    frontTarget(true),
    _dir(dir)
{
    ui->setupUi(this);

    setWindowTitle("SRFRS");

    // set up deck label
    ui->lblDeck->adjustSize();

    // set up combobox
    ui->cbDecks->insertItems(0, decks);

    // disable tab as input, make it change text boxes
    ui->txtFront->setTabChangesFocus(true);
    ui->txtBack->setTabChangesFocus(true);

    // give focus to deck chooser
    ui->cbDecks->setFocus();

    // set up validation
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->buttonBox->setToolTip("Flashcard can't be empty");
    ui->txtFront->setStyleSheet("QTextEdit { color: red }");
    ui->txtBack->setStyleSheet("QTextEdit { color: red }");

    // disable image button initially
    ui->btnAddImage->setEnabled(false);

    // set up event filter
    ui->txtFront->installEventFilter(this);
    ui->txtBack->installEventFilter(this);

    // init stuff to flashcard information
    ui->cbDecks->setCurrentIndex(ui->cbDecks->findText(_flashcard->getDeck()));
    ui->txtFront->setText(_flashcard->getFront().join("\n"));
    ui->txtBack->setText(_flashcard->getBack().join("\n"));
}

FlashcardEditor::~FlashcardEditor()
{
    delete ui;
}

MainWindow* FlashcardEditor::getParent() {
    return dynamic_cast<MainWindow*>(parent());
}

void FlashcardEditor::on_buttonBox_accepted()
{
    // write to file
    getParent()->getFlashcardManager().updateFront(_flashcard->getID(), ui->txtFront->toPlainText());
    getParent()->getFlashcardManager().updateBack(_flashcard->getID(), ui->txtBack->toPlainText());
    getParent()->getFlashcardManager().update(_flashcard->getID(), 1, ui->cbDecks->currentText());

    // update flashcard
    _flashcard->setFront(ui->txtFront->toPlainText().split("\n"));
    _flashcard->setBack(ui->txtBack->toPlainText().split("\n"));

    if(_flashcard->getDeck() != ui->cbDecks->currentText()) {

        // validate changing deck will reset interval
        if (QMessageBox::Yes == QMessageBox(QMessageBox::Warning, "SRFRS", "Changing decks will reset the review interval for this flashcard. Are you sure you want to continue?", QMessageBox::Yes|QMessageBox::No, getParent()).exec()) {
            getParent()->getDeckManager().getDeck(_flashcard->getDeck())->removeCard(_flashcard);

            _flashcard->setDeck(ui->cbDecks->currentText());
            _flashcard->setReviewDate(QDate::currentDate());

            // initialise initial interval and difficulty to 1 and 2.5 respectively
            _flashcard->setRepetitions(0);
            _flashcard->setInterval(1);
            _flashcard->setDifficulty(2.5f);

            // write to file
            getParent()->getFlashcardManager().update(_flashcard->getID(), 2, _flashcard->getReviewDate().toString("dd/MM/yyyy"));
            getParent()->getFlashcardManager().update(_flashcard->getID(), 3, QString::number(0));
            getParent()->getFlashcardManager().update(_flashcard->getID(), 4, QString::number(1));
            getParent()->getFlashcardManager().update(_flashcard->getID(), 5, QString::number(2.5f));

            // add flashcard to deck
            getParent()->getDeckManager().getDeck(_flashcard->getDeck())->addCard(_flashcard);
        } else {
            ui->cbDecks->setFocus();
            return;
        }
    }

    done(QDialog::Accepted);
}

void FlashcardEditor::on_txtFront_textChanged()
{
    validateInputs();
}

void FlashcardEditor::on_txtBack_textChanged()
{
    validateInputs();
}

void FlashcardEditor::on_btnAddImage_clicked()
{
    // get the target text edit
    QTextEdit *_target;
    frontTarget ? _target = ui->txtFront : _target = ui->txtBack;

    // choose image
    QString filePath = QFileDialog::getOpenFileName(this, "Open Image", QDir::homePath(), "Image (*.png *.jpg *.gif)");
    QString fileName;

    // get the file name
    if(!filePath.isEmpty()) fileName = filePath.split("/").back();

    // copy image to res folder (in AppData/Local/qtSRFRS)
    if(!filePath.isEmpty()) QFile::copy(filePath, _dir + fileName);

    // insert image into text edit
    if(!fileName.isEmpty()) _target->setText(_target->toPlainText() + "[" + fileName + "]");
}

bool FlashcardEditor::eventFilter(QObject *obj, QEvent *ev)
{
    if(ev->type() == ev->FocusIn && obj == ui->txtFront) {          // check if front text edit
                                                                    // gains focus
        // enable the add image button
        ui->btnAddImage->setEnabled(true);
        // target is front
        frontTarget = true;
    } else if(ev->type() == ev->FocusIn && obj == ui->txtBack) {    // check if back text edit
                                                                    // gains focused
        // enable the add image button
        ui->btnAddImage->setEnabled(true);
        // target is back
        frontTarget = false;
    } if(ev->type() == ev->FocusOut && obj == ui->txtFront) {       // check if front text edit
                                                                    // loses focus
        // disable the add image button
        ui->btnAddImage->setEnabled(false);
        // target is front
        frontTarget = true;
    } else if(ev->type() == ev->FocusOut && obj == ui->txtBack) {   // check if back text edit
                                                                    // loses focus
        // disable the add image button
        ui->btnAddImage->setEnabled(false);
        // target is back
        frontTarget = false;
    }

    return false;
}

void FlashcardEditor::validateInputs()
{
    // get front and back text
    QString frontText = ui->txtFront->toPlainText();
    QString backText = ui->txtBack->toPlainText();

    // check if any changes to the flashcard front have occurred
    if(frontText != _flashcard->getFront().join("\n")) {
        ui->txtFront->setStyleSheet("QTextEdit { color: blue }");
        ui->txtFront->setToolTip("Value changed");
    } else {
        ui->txtFront->setStyleSheet("");
        ui->txtFront->setToolTip("");
    }

    // check if any changes to the flashcard back have occurred
    if(backText != _flashcard->getBack().join("\n")) {
        ui->txtBack->setStyleSheet("QTextEdit { color: blue }");
        ui->txtBack->setToolTip("Value changed");
    } else {
        ui->txtBack->setStyleSheet("");
        ui->txtBack->setToolTip("");
    }

    // check if flashcard front and back are valid
    bool front = validText(ui->txtFront);
    bool back = validText(ui->txtBack);
    if(front && back) {
        // enable OK button
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        ui->buttonBox->setToolTip("");
    }
}

bool FlashcardEditor::validText(QTextEdit *edit)
{
    // get text to validate
    QString text = edit->toPlainText();

    QRegExp imagePattern = QRegExp("\\[((.*)\\.(jpg|png|gif))\\]");
    imagePattern.indexIn(text);

    // check if text contains ";;"
    if(text.contains(";;")) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->buttonBox->setToolTip("Flashcard can't contain ;;");
        edit->setToolTip("Flashcard can't contain ;;");
        edit->setStyleSheet("QTextEdit { color: red }");

        return false;

    // check if text is empty
    } else if(text.isEmpty()) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->buttonBox->setToolTip("Flashcard can't be empty");
        edit->setToolTip("Flashcard can't be empty");
        edit->setStyleSheet("QTextEdit { color: red }");

        return false;

    // check if text is longer than 12 lines
    } else if(edit->document()->blockCount() > 12) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->buttonBox->setToolTip("Flashcard is too long");
        edit->setToolTip("Flashcard is too long");
        edit->setStyleSheet("QTextEdit { color: red }");

        return false;

    // check if text contains an image and text
    } else if(text.contains(imagePattern) && text != "[" + imagePattern.cap(1) + "]") {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->buttonBox->setToolTip("Flashcard side can't contain both image and text");
        edit->setToolTip("Flashcard side can't contain both image and text");
        edit->setStyleSheet("QTextEdit { color: red }");

        return false;
    } else {
        if(edit->styleSheet() != "QTextEdit { color: blue }") {
            // reset stylesheet and tooltip
            edit->setStyleSheet("");
            edit->setToolTip("");
        }
        return true;
    }
}

void FlashcardEditor::on_buttonBox_rejected()
{
    // close this form
    done(QDialog::Rejected);
}

void FlashcardEditor::on_cbDecks_currentIndexChanged(const QString &string)
{
    // check if selected deck is different to initial deck
    if(string != _flashcard->getDeck()) {
        //
        ui->cbDecks->setStyleSheet("QComboBox { color: blue }");
        ui->cbDecks->setToolTip("Value changed");
    } else {
        // reset stylesheet and tool tip
        ui->cbDecks->setStyleSheet("");
        ui->cbDecks->setToolTip("");
    }
}
