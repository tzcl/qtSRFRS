#include "flashcardeditor.h"
#include "ui_flashcardeditor.h"

#include <QRegExp>
#include <QFile>
#include <QFileDialog>
#include <QDebug>

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
    ui->lbl_deck->adjustSize();

    // set up combobox
    ui->cb_decks->insertItems(0, decks);

    // disable tab as input, make it change text boxes
    ui->txt_front->setTabChangesFocus(true);
    ui->txt_back->setTabChangesFocus(true);

    // give focus to deck chooser
    ui->cb_decks->setFocus();

    // set up validation
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->buttonBox->setToolTip("Flashcard can't be empty");
    ui->txt_front->setStyleSheet("background: red");
    ui->txt_back->setStyleSheet("background: red");

    // disable image button initially
    ui->btn_add_image->setEnabled(false);

    // set up event filter
    ui->txt_front->installEventFilter(this);
    ui->txt_back->installEventFilter(this);

    // init stuff to flashcard information
    ui->cb_decks->setCurrentIndex(ui->cb_decks->findText(_flashcard->getDeck()));
    ui->txt_front->setText(_flashcard->getFront().join("\n"));
    ui->txt_back->setText(_flashcard->getBack().join("\n"));
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
    getParent()->getFlashcardManager().updateFront(_flashcard->getID(), ui->txt_front->toPlainText());
    getParent()->getFlashcardManager().updateBack(_flashcard->getID(), ui->txt_back->toPlainText());
    getParent()->getFlashcardManager().update(_flashcard->getID(), 1, ui->cb_decks->currentText());

    // update flashcard
    _flashcard->setFront(ui->txt_front->toPlainText().split("\n"));
    _flashcard->setBack(ui->txt_back->toPlainText().split("\n"));

    if(_flashcard->getDeck() != ui->cb_decks->currentText()) {
        getParent()->getDeckManager().getDeck(_flashcard->getDeck())->removeCard(_flashcard);
        _flashcard->setDeck(ui->cb_decks->currentText());
        getParent()->getDeckManager().getDeck(_flashcard->getDeck())->addCard(_flashcard);
    }

    getParent()->updateTables();

    done(QDialog::Accepted);
}

void FlashcardEditor::on_txt_front_textChanged()
{
    validateInputs();
}

void FlashcardEditor::on_txt_back_textChanged()
{
    validateInputs();
}

void FlashcardEditor::on_btn_add_image_clicked()
{
    QTextEdit *_target;
    frontTarget ? _target = ui->txt_front : _target = ui->txt_back;

    // choose image
    QString filePath = QFileDialog::getOpenFileName(this, "Open Image", QDir::homePath(), "Image (*.png *.jpg *.gif)");
    QString fileName;
    if(!filePath.isEmpty()) fileName = filePath.split("/").back();

    // copy image to res folder (in AppData/Local/qtSRFRS)
    QFile::copy(filePath, _dir + fileName);

    if(!fileName.isEmpty()) _target->setText(_target->toPlainText() + "[" + fileName + "]");
}

bool FlashcardEditor::eventFilter(QObject *obj, QEvent *ev)
{
    if(ev->type() == ev->FocusIn && obj == ui->txt_front) {
        frontTarget = true;
        ui->btn_add_image->setEnabled(true);
    } else if(ev->type() == ev->FocusIn && obj == ui->txt_back) {
        frontTarget = false;
        ui->btn_add_image->setEnabled(true);
    } if(ev->type() == ev->FocusOut && obj == ui->txt_front) {
        frontTarget = true;
        ui->btn_add_image->setEnabled(false);
    } else if(ev->type() == ev->FocusOut && obj == ui->txt_back) {
        frontTarget = false;
        ui->btn_add_image->setEnabled(false);
    }

    return false;
}

void FlashcardEditor::validateInputs()
{
    QString frontText = ui->txt_front->toPlainText();
    QString backText = ui->txt_back->toPlainText();

    // check if any changes have occurred
    if(frontText != _flashcard->getFront().join("\n")) {
        ui->txt_front->setStyleSheet("background: yellow");
        ui->txt_front->setToolTip("Value changed");
    } else {
        ui->txt_front->setStyleSheet("");
        ui->txt_front->setToolTip("");
    }

    if(backText != _flashcard->getBack().join("\n")) {
        ui->txt_back->setStyleSheet("background: yellow");
        ui->txt_back->setToolTip("Value changed");
    } else {
        ui->txt_back->setStyleSheet("");
        ui->txt_back->setToolTip("");
    }

    if(validText(ui->txt_front) && validText(ui->txt_back)) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        ui->buttonBox->setToolTip("");
    }
}

bool FlashcardEditor::validText(QTextEdit *edit)
{
    // validate text
    QString text = edit->toPlainText();

    QRegExp imagePattern = QRegExp("\\[((.*)\\.(jpg|png|gif))\\]");
    imagePattern.indexIn(text);

    if(text.contains(";;")) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->buttonBox->setToolTip("Flashcard can't contain ;;");
        edit->setToolTip("Flashcard can't contain ;;");
        edit->setStyleSheet("background: red");

        return false;
    } else if(text.isEmpty()) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->buttonBox->setToolTip("Flashcard can't be empty");
        edit->setToolTip("Flashcard can't be empty");
        edit->setStyleSheet("background: red");

        return false;
    } else if(edit->document()->blockCount() > 12) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->buttonBox->setToolTip("Flashcard is too long");
        edit->setToolTip("Flashcard is too long");
        edit->setStyleSheet("background: red");

        return false;
    } else if(text.contains(imagePattern) && text != "[" + imagePattern.cap(1) + "]") {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->buttonBox->setToolTip("Flashcard side can't contain both image and text");
        edit->setToolTip("Flashcard side can't contain both image and text");
        edit->setStyleSheet("background: red");

        return false;
    } else {
        if(edit->styleSheet() != "background: yellow") {
            edit->setStyleSheet("");
            edit->setToolTip("");
        }
        return true;
    }
}

void FlashcardEditor::on_buttonBox_rejected()
{
    done(QDialog::Rejected);
}

void FlashcardEditor::on_cb_decks_currentIndexChanged(const QString &string)
{
    if(string != _flashcard->getDeck()) {
        ui->cb_decks->setStyleSheet("background: yellow");
        ui->cb_decks->setToolTip("Value changed");
    } else {
        ui->cb_decks->setStyleSheet("");
        ui->cb_decks->setToolTip("");
    }
}
