#include "flashcardcreator.h"
#include "ui_flashcardcreator.h"

#include "flashcard.h"

#include <QDebug>
#include <QDate>
#include <QFileDialog>
#include <QDir>
#include <QTextBlock>
#include <QTextCursor>
#include <QRegExp>

FlashcardCreator::FlashcardCreator(QString dir, QStringList decks, QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint),
    ui(new Ui::FlashcardCreator),
    _dir(dir + "/res/")
{
    ui->setupUi(this);

    setWindowTitle("SRFRS");

    // set up res folder
    QDir directory(_dir);
    if(!directory.exists()) directory.mkpath(directory.path());

    ui->lbl_deck->adjustSize();

    ui->cb_decks->insertItems(0, decks);

    // disable tab as input, make it change text boxes
    ui->txt_front->setTabChangesFocus(true);
    ui->txt_back->setTabChangesFocus(true);

    // give focus to deck chooser
    ui->cb_decks->setFocus();

    // set up validation
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->buttonBox->setToolTip("Flashcard can't be empty");
    ui->txt_front->setToolTip("Flashcard can't be empty");
    ui->txt_front->setStyleSheet("background: red");
    ui->txt_back->setStyleSheet("background: red");
    ui->txt_back->setToolTip("Flashcard can't be empty");

    // disable image button initially
    ui->btn_add_image->setEnabled(false);

    // set up event filter
    ui->txt_front->installEventFilter(this);
    ui->txt_back->installEventFilter(this);
}

FlashcardCreator::~FlashcardCreator()
{
    delete ui;
}

MainWindow* FlashcardCreator::getParent() {
    return dynamic_cast<MainWindow*>(parent());
}

bool FlashcardCreator::eventFilter(QObject *obj, QEvent *ev)
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

void FlashcardCreator::on_buttonBox_accepted()
{
    QStringList front = ui->txt_front->toPlainText().split("\n");
    QStringList back = ui->txt_back->toPlainText().split("\n");

    getParent()->addFlashcard(getParent()->getFlashcardManager().getValidID(), front, back, ui->cb_decks->currentText());
}

bool FlashcardCreator::validText(QTextEdit *edit)
{
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
        edit->setStyleSheet("");

        return true;
    }
}

void FlashcardCreator::validateInputs()
{
    if(validText(ui->txt_front) && validText(ui->txt_back)) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        ui->buttonBox->setToolTip("");
    }
}

void FlashcardCreator::on_txt_front_textChanged()
{
    validateInputs();
}

void FlashcardCreator::on_txt_back_textChanged()
{
    validateInputs();
}

void FlashcardCreator::on_btn_add_image_clicked()
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
