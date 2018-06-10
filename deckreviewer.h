#ifndef DECKREVIEWER_H
#define DECKREVIEWER_H

#include <QDialog>
#include <QSharedPointer>
#include <QString>
#include <QRegExp>
#include <QVector>

#include "deck.h"
#include "flashcard.h"
#include "flashcardmanager.h"

namespace Ui {
class DeckReviewer;
}

class DeckReviewer : public QDialog
{
    Q_OBJECT

public:
    explicit DeckReviewer(QString dirPath, SRFRS::FlashcardManager flashcardManager, QSharedPointer<SRFRS::Deck> deck, QWidget *parent = 0);
    ~DeckReviewer();

    /**
     * @brief incrementIndex: Increment the index and update GUI.
     */
    void incrementIndex();

    /**
     * @brief showDifficultyButtons: Set the state of the difficulty buttons to the state of active.
     * @param active: Whether the buttons should be active or not.
     */
    void showDifficultyButtons(bool active);

    /**
     * @brief handleReview: Handle the review of a flashcard.
     * @param difficulty: The user difficulty in the review.
     */
    void handleReview(int difficulty);

    /**
     * @brief handleFlashcard: Loads the flashcard front and back.
     */
    void handleFlashcard();

private slots:
    void on_btnEasy_clicked();

    void on_btnShow_clicked();

    void on_btnGood_clicked();

    void on_btnHard_clicked();

    void on_btnClose_clicked();

    void on_btnPrettyEasy_clicked();

    void on_btnPrettyHard_clicked();

private:
    Ui::DeckReviewer *ui;

    QString _dirPath;
    SRFRS::FlashcardManager &_flashcardManager;

    QSharedPointer<SRFRS::Deck> _deck;
    QVector<QSharedPointer<SRFRS::Flashcard>> _review;

    int _index;
    QString _size;
    QRegExp _imagePattern;
};

#endif // DECKREVIEWER_H
