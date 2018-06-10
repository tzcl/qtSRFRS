#ifndef DECKRENAMER_H
#define DECKRENAMER_H

#include <QDialog>
#include <QStringList>
#include <QSharedPointer>

#include "deck.h"
#include "mainwindow.h"

namespace Ui {
class DeckRenamer;
}

class DeckRenamer : public QDialog
{
    Q_OBJECT

public:
    explicit DeckRenamer(QSharedPointer<SRFRS::Deck> deck, QWidget *parent = 0);
    ~DeckRenamer();

private slots:
    void on_buttonBox_accepted();

    void on_txtName_textEdited(const QString &string);

private:
    MainWindow *getParent();

    Ui::DeckRenamer *ui;

    QSharedPointer<SRFRS::Deck> _deck;
};

#endif // DECKRENAMER_H
