#ifndef DECKRENAMER_H
#define DECKRENAMER_H

#include <QDialog>
#include <QStringList>

#include "deck.h"
#include "mainwindow.h"

namespace Ui {
class DeckRenamer;
}

class DeckRenamer : public QDialog
{
    Q_OBJECT

public:
    explicit DeckRenamer(SRFRS::Deck &deck, QWidget *parent = 0);
    ~DeckRenamer();

private slots:
    void on_buttonBox_accepted();

    void on_txt_name_textEdited(const QString &string);

private:
    MainWindow *getParent();

    Ui::DeckRenamer *ui;

    SRFRS::Deck &_deck;
};

#endif // DECKRENAMER_H
