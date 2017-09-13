#ifndef FLASHCARDCREATOR_H
#define FLASHCARDCREATOR_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QTextEdit>

#include "flashcard.h"
#include "mainwindow.h"

namespace Ui {
class FlashcardCreator;
}

class FlashcardCreator : public QDialog
{
    Q_OBJECT

public:
    explicit FlashcardCreator(QStringList decks, QWidget *parent = 0);
    ~FlashcardCreator();

    MainWindow* getParent();

private slots:
    void on_buttonBox_accepted();

    void on_txt_front_textChanged();

    void on_txt_back_textChanged();

private:
    void validateInputs();
    bool validText(QTextEdit *edit);

    Ui::FlashcardCreator *ui;
};

#endif // FLASHCARDCREATOR_H
