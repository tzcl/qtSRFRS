#ifndef FLASHCARDCREATOR_H
#define FLASHCARDCREATOR_H

#include <QDialog>
#include <QStringList>

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

private:
    Ui::FlashcardCreator *ui;
};

#endif // FLASHCARDCREATOR_H
