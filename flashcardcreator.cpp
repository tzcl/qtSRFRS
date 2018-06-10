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

    ui->lblDeck->adjustSize();

    ui->cbDecks->insertItems(0, decks);

    // disable tab as input, make it change text boxes
    ui->txtFront->setTabChangesFocus(true);
    ui->txtBack->setTabChangesFocus(true);

    // give focus to deck chooser
    ui->cbDecks->setFocus();

    // set up validation
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->buttonBox->setToolTip("Flashcard can't be empty");

    ui->txtFront->setToolTip("Flashcard can't be empty");
    ui->txtFront->setStyleSheet("QTextEdit#txtFront { color: red }");
    ui->txtBack->setStyleSheet("QTextEdit#txtBack { color: red }");
    ui->txtBack->setToolTip("Flashcard can't be empty");

    // disable image button initially
    ui->btnAddImage->setEnabled(false);

    // set up event filter
    ui->txtFront->installEventFilter(this);
    ui->txtBack->installEventFilter(this);
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

void FlashcardCreator::on_buttonBox_accepted()
{
    // get flashcard front and back
    QStringList front = ui->txtFront->toPlainText().split("\n");
    QStringList back = ui->txtBack->toPlainText().split("\n");

    // add new flashcard to system
    getParent()->addFlashcard(getParent()->getFlashcardManager().getValidID(), front, back, ui->cbDecks->currentText());
}

bool FlashcardCreator::validText(QTextEdit *edit)
{
    // get text
    QString text = edit->toPlainText();
    QString specifier = "#" + edit->objectName();

    QRegExp imagePattern = QRegExp("\\[((.*)\\.(jpg|png|gif))\\]");
    imagePattern.indexIn(text);

    if(text.contains(";;")) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->buttonBox->setToolTip("Flashcard can't contain ;;");
        edit->setToolTip("Flashcard can't contain ;;");
        edit->setStyleSheet("QTextEdit" + specifier + "{ color: red }");

        return false;
    } else if(text.isEmpty()) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->buttonBox->setToolTip("Flashcard can't be empty");
        edit->setToolTip("Flashcard can't be empty");
        edit->setStyleSheet("QTextEdit" + specifier + "{ color: red }");

        return false;
    } else if(edit->document()->blockCount() > 12) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->buttonBox->setToolTip("Flashcard is too long");
        edit->setToolTip("Flashcard is too long");
        edit->setStyleSheet("QTextEdit" + specifier + "{ color: red }");

        return false;
    } else if(text.contains(imagePattern) && text != "[" + imagePattern.cap(1) + "]") {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->buttonBox->setToolTip("Flashcard side can't contain both image and text");
        edit->setToolTip("Flashcard side can't contain both image and text");
        edit->setStyleSheet("QTextEdit" + specifier + "{ color: red }");

        return false;
    } else {
        edit->setStyleSheet("");

        return true;
    }
}

void FlashcardCreator::validateInputs()
{
    // check if flashcard front and back are valid
    bool front = validText(ui->txtFront);
    bool back = validText(ui->txtBack);
    if(front && back) {
        // enable OK button
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        ui->buttonBox->setToolTip("");
    }
}

void FlashcardCreator::on_txtFront_textChanged()
{
    validateInputs();
}

void FlashcardCreator::on_txtBack_textChanged()
{
    validateInputs();
}

void FlashcardCreator::on_btnAddImage_clicked()
{
    // get target
    QTextEdit *_target;
    frontTarget ? _target = ui->txtFront : _target = ui->txtBack;

    // choose image
    QString filePath = QFileDialog::getOpenFileName(this, "Open Image", QDir::homePath(), "Image (*.png *.jpg *.gif)");
    QString fileName;
    if(!filePath.isEmpty()) fileName = filePath.split("/").back();

    // copy image to res folder (in AppData/Local/qtSRFRS)
    if(!filePath.isEmpty()) QFile::copy(filePath, _dir + fileName);

    // insert image into target
    if(!fileName.isEmpty()) _target->setText(_target->toPlainText() + "[" + fileName + "]");
}
