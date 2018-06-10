#ifndef FLASHCARDEDITOR_H
#define FLASHCARDEDITOR_H

#include <QDialog>
#include <QSharedPointer>
#include <QTextEdit>

#include "flashcard.h"
#include "mainwindow.h"

namespace Ui {
class FlashcardEditor;
}

class FlashcardEditor : public QDialog
{
    Q_OBJECT

public:
    explicit FlashcardEditor(QString dir, QSharedPointer<SRFRS::Flashcard> flashcard, QStringList decks, QWidget *parent = 0);
    ~FlashcardEditor();

    MainWindow* getParent();

private slots:
    void on_buttonBox_accepted();

    void on_txtFront_textChanged();

    void on_txtBack_textChanged();

    bool eventFilter(QObject *obj, QEvent *ev);

    void on_btnAddImage_clicked();

    void on_buttonBox_rejected();

    void on_cbDecks_currentIndexChanged(const QString &string);

private:
    void validateInputs();
    bool validText(QTextEdit *edit);

    Ui::FlashcardEditor *ui;

    QSharedPointer<SRFRS::Flashcard> _flashcard;

    bool frontTarget;
    QString _dir;
};

#endif // FLASHCARDEDITOR_H
