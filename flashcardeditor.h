#ifndef FLASHCARDEDITOR_H
#define FLASHCARDEDITOR_H

#include <QDialog>

namespace Ui {
class FlashcardEditor;
}

class FlashcardEditor : public QDialog
{
    Q_OBJECT

public:
    explicit FlashcardEditor(QWidget *parent = 0);
    ~FlashcardEditor();

private:
    Ui::FlashcardEditor *ui;
};

#endif // FLASHCARDEDITOR_H
