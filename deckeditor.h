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
    explicit DeckEditor(QSharedPointer<SRFRS::Deck> deck, QVector<QSharedPointer<SRFRS::Flashcard>> flashcards, QWidget *parent = 0);
    ~DeckEditor();

    MainWindow *getParent();

private slots:

private:
    Ui::DeckEditor *ui;

    QSharedPointer<SRFRS::Deck> _deck;
    QVector<QSharedPointer<SRFRS::Flashcard>> _flashcards;
};

#endif // DECKEDITOR_H
