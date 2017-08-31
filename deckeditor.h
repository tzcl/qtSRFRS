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
    explicit DeckEditor(QString name, SRFRS::Deck &deck, QWidget *parent = 0);
    ~DeckEditor();

private slots:

private:
    Ui::DeckEditor *ui;

    SRFRS::Deck &_deck;
};

#endif // DECKEDITOR_H
