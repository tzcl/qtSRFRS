#ifndef DECKEDITOR_H
#define DECKEDITOR_H

#include <QDialog>
#include <QVector>
#include <QSharedPointer>

#include "deck.h"
#include "flashcard.h"
#include "mainwindow.h"

namespace Ui {
class DeckEditor;
}

class DeckEditor : public QDialog
{
    Q_OBJECT

public:
    explicit DeckEditor(QSharedPointer<SRFRS::Deck> deck, QWidget *parent = 0);
    ~DeckEditor();

    MainWindow *getParent();

    void addFlashcardToTable(int row, SRFRS::Flashcard card);

    void addFlashcardToTable(SRFRS::Flashcard card);

    void addFlashcardButton(int row, int id);

    int getFlashcardRow(int id);

    void resetIDs();

private slots:

    void on_txt_name_textEdited(const QString &string);

    void flashcard_preview(int ID);

    void flashcard_edit(int ID);

    void flashcard_delete(int ID);

    void on_flashcards_table_cellDoubleClicked(int row, int column);

    void on_btn_add_card_clicked();

    void on_buttonBox_accepted();

private:
    Ui::DeckEditor *ui;

    QSharedPointer<SRFRS::Deck> _deck;
};

#endif // DECKEDITOR_H
