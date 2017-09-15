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
    explicit FlashcardEditor(QString dir, QSharedPointer<SRFRS::Flashcard> flashcard, QVector<QSharedPointer<SRFRS::Flashcard>> flashcards, QStringList decks, QWidget *parent = 0);
    ~FlashcardEditor();
    MainWindow* getParent();

private slots:
    void on_buttonBox_accepted();

    void on_txt_front_textChanged();

    void on_txt_back_textChanged();

    bool eventFilter(QObject *obj, QEvent *ev);

    void on_btn_add_image_clicked();

    void on_buttonBox_rejected();

    void on_cb_decks_currentIndexChanged(const QString &string);

private:
    void validateInputs();
    bool validText(QTextEdit *edit);

    Ui::FlashcardEditor *ui;

    QSharedPointer<SRFRS::Flashcard> _flashcard;
    QVector<QSharedPointer<SRFRS::Flashcard>> _flashcards;

    bool frontTarget;
    QString _dir;
};

#endif // FLASHCARDEDITOR_H
