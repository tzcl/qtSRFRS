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

    /**
     * @brief addFlashcardToTable: Add flashcard to the flashcards table
     * @param row: The row to add the flashcard to.
     * @param card: The flashcard to add.
     */
    void addFlashcardToTable(int row, SRFRS::Flashcard card);

    /**
     * @brief addFlashcardToTable: Overload of addFlashcardToTable, adds a new row
     *                             and inserts flashcard into that.
     * @param card: The flashcard to add.
     */
    void addFlashcardToTable(SRFRS::Flashcard card);

    /**
     * @brief addFlashcardButton: Add a button to the flashcard table.
     * @param row: The row to add the button to.
     * @param id: The ID of the flashcard to link with the button.
     */
    void addFlashcardButton(int row, int id);

    /**
     * @brief getFlashcardRow: Finds the row of a flashcard in the table.
     * @param id: The ID of the flashcard.
     * @return: The row the flashcard is in.
     */
    int getFlashcardRow(int id);

    /**
     * @brief resetIDs: Reset the IDs of flashcards in the table.
     */
    void resetIDs();

private slots:

    void on_txtName_textEdited(const QString &string);

    void flashcard_preview(int ID);

    void flashcard_edit(int ID);

    void flashcard_delete(int ID);

    void on_flashcardsTable_cellDoubleClicked(int row, int column);

    void on_btnAddCard_clicked();

    void on_buttonBox_accepted();

private:
    Ui::DeckEditor *ui;

    QSharedPointer<SRFRS::Deck> _deck;
};

#endif // DECKEDITOR_H
