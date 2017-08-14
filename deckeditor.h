#ifndef DECKEDITOR_H
#define DECKEDITOR_H

#include <QDialog>

#include "deck.h"

namespace Ui {
class DeckEditor;
}

class DeckEditor : public QDialog
{
    Q_OBJECT

public:
    explicit DeckEditor(SRFRS::Deck &deck, QWidget *parent = 0);
    ~DeckEditor();

private slots:
    void on_btn_rename_clicked();

    void on_btn_delete_clicked();

private:
    Ui::DeckEditor *ui;

    SRFRS::Deck &_deck;
};

#endif // DECKEDITOR_H
