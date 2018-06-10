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
    explicit FlashcardCreator(QString dir, QStringList decks, QWidget *parent = 0);
    ~FlashcardCreator();

    MainWindow* getParent();

private slots:
    void on_buttonBox_accepted();

    void on_txtFront_textChanged();

    void on_txtBack_textChanged();

    bool eventFilter(QObject *obj, QEvent *ev);

    void on_btnAddImage_clicked();

private:
    void validateInputs();
    bool validText(QTextEdit *edit);

    Ui::FlashcardCreator *ui;

    bool frontTarget;
    QString _dir;
};

#endif // FLASHCARDCREATOR_H
