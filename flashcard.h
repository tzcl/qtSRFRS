#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QString>
#include <QStringList>
#include <QDate>

namespace SRFRS {

class Flashcard
{
public:
    Flashcard();
    Flashcard(int ID, QStringList front, QStringList back, QString deck, QDate reviewDate, int repetitions, int previousInterval, float difficultyCoefficient);

    /**
     * @brief getID: Returns the ID of the flashcard.
     * @return: The ID of the flashcard.
     */
    int getID() { return _ID; }

    /**
     * @brief setID: Sets the ID of the flashcard to id.
     * @param id: The new id of the flashcard.
     */
    void setID(int id) { _ID = id; }

    /**
     * @brief getFront: Returns the front of the flashcard.
     * @return: The front of the flashcard.
     */
    QStringList getFront() { return _frontText; }

    /**
     * @brief setFront: Sets the front of the flashcard to front.
     * @param front: The new front of the flashcard.
     */
    void setFront(QStringList front) { _frontText = front; }

    /**
     * @brief getBack: Returns the back of the flashcard.
     * @return: The back of the flashcard.
     */
    QStringList getBack() { return _backText; }

    /**
     * @brief setBack: Sets the back of the flashcard to back.
     * @param back: The new back of the flashcard.
     */
    void setBack(QStringList back) { _backText = back; }

    /**
     * @brief getDeck: Returns the name of the deck containing the flashcard.
     * @return: The name of the deck containing the flashcard.
     */
    QString getDeck() { return _deck; }

    /**
     * @brief setDeck: Sets the deck of the flashcard to deck.
     * @param deck: The new deck of the flashcard.
     */
    void setDeck(QString deck) { _deck = deck; }

    /**
     * @brief getReviewDate: Returns the review date of the flashcard.
     * @return: The review date of the flashcard.
     */
    QDate getReviewDate() { return _reviewDate; }

    /**
     * @brief setReviewDate: Sets the review of the flashcard to date.
     * @param date: The new review date of the flashcard.
     */
    void setReviewDate(QDate date) { _reviewDate = date; }

    /**
     * @brief getRepetitions: Returns the flashcard's number of repetitions.
     * @return: The flashcard's number of repetitions.
     */
    int getRepetitions() { return _repetitions; }

    /**
     * @brief setRepetitions: Sets the number of flashcard repetitions to repetitions.
     * @param repetitions: The new number of repetitions.
     */
    void setRepetitions(int repetitions) { _repetitions = repetitions; }

    /**
     * @brief getInterval: Get the previous review interval for the flashcard.
     * @return: The previous review interval for the flashcard.
     */
    int getInterval() { return _previousInterval; }

    /**
     * @brief setInterval: Sets the previous review interval for the flashcard to interval.
     * @param interval: The new interval for the flashcard.
     */
    void setInterval(int interval) { _previousInterval = interval; }

    /**
     * @brief getDifficultyCoefficient: Returns the flashcard's difficulty coefficient.
     * @return: The flashcard's difficulty coefficient.
     */
    float getDifficultyCoefficient() { return _difficultyCoefficient; }

    /**
     * @brief setDifficulty: Sets the flashcard's difficulty coefficient to diff.
     * @param diff: The new difficulty coefficient of the flashcard.
     */
    void setDifficulty(float diff) { _difficultyCoefficient = diff; }

private:

    int _ID;
    QString _deck;
    QDate _reviewDate;

    QStringList _frontText;
    QStringList _backText;

    int _repetitions;
    int _previousInterval;
    float _difficultyCoefficient;
};

}

#endif // FLASHCARD_H
