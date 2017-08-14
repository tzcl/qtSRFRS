#ifndef FLASHCARDCREATOR_H
#define FLASHCARDCREATOR_H

#include <QDialog>

#include "flashcard.h"

namespace Ui {
class FlashcardCreator;
}

class FlashcardCreator : public QDialog
{
    Q_OBJECT

public:
    explicit FlashcardCreator(QWidget *parent = 0);
    ~FlashcardCreator();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::FlashcardCreator *ui;
};

#endif // FLASHCARDCREATOR_H
