#ifndef FLASHCARDCREATOR_H
#define FLASHCARDCREATOR_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QTextEdit>
#include <QObject>
#include <QVector>
#include <QSharedPointer>

#include "flashcard.h"
#include "mainwindow.h"

namespace Ui {
class FlashcardCreator;
}

class FlashcardCreator : public QDialog
{
    Q_OBJECT

public:
    explicit FlashcardCreator(QString dir, QStringList decks, QVector<QSharedPointer<SRFRS::Flashcard>> flashcards, QWidget *parent = 0);
    ~FlashcardCreator();

    MainWindow* getParent();

private slots:
    void on_buttonBox_accepted();

    void on_txt_front_textChanged();

    void on_txt_back_textChanged();

    bool eventFilter(QObject *obj, QEvent *ev);

    void on_btn_add_image_clicked();

private:
    void validateInputs();
    bool validText(QTextEdit *edit);

    Ui::FlashcardCreator *ui;

    QVector<QSharedPointer<SRFRS::Flashcard>> _flashcards;

    bool frontTarget;
    QString _dir;
};

#endif // FLASHCARDCREATOR_H
