#ifndef FLASHCARDPREVIEWER_H
#define FLASHCARDPREVIEWER_H

#include <QDialog>

#include "flashcard.h"

namespace Ui {
class FlashcardPreviewer;
}

class FlashcardPreviewer : public QDialog
{
    Q_OBJECT

public:
    explicit FlashcardPreviewer(QString dirPath, SRFRS::Flashcard flashcard, QWidget *parent = 0);
    ~FlashcardPreviewer();

private slots:
    void on_btn_toggle_clicked();

private:
    Ui::FlashcardPreviewer *ui;

    bool _show;
};

#endif // FLASHCARDPREVIEWER_H
