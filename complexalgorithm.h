#ifndef COMPLEXALGORITHM_H
#define COMPLEXALGORITHM_H

#include <QDate>

namespace SRFRS {

class ComplexAlgorithm
{
public:

    // Built upon the SM-2 algorithm by Piotr Wozniak
    // See: https://www.supermemo.com/english/ol/sm2.htm

    /**
     * @brief schedule: Schedule the review interval for a particular flashcard.
     * @param repetitions: number of times the flashcard has been reviewed.
     * @param previousInterval: the previous interval
     * @return: The optimal interval (in number of days) until the next review session.
     */
    static int schedule(int repetitions, int previousInterval, float difficultyCoefficient) {
        if(repetitions == 0) {
            return 1;
        } else if(repetitions == 1) {
            return 6;
        } else {
            return previousInterval * difficultyCoefficient;
        }
    }

    /**
     * @brief calculateCoefficient: Calculate the difficult coefficient of a flashcard
     *                              given its previous difficulty and user difficulty
     *                              in its most recent review.
     * @param previousDifficultyCoefficient: the previous difficulty coefficient.
     * @param difficulty: the difficulty of the flashcard in its most recent review.
     * @return: The updated difficulty coefficient for the flashcard.
     */
    static float calculateCoefficient(float previousDifficultyCoefficient, int difficulty) {
        return previousDifficultyCoefficient + (0.1 - (5 - difficulty) * (0.08 + (5 - difficulty) * 0.02));
    }
};

}

#endif // COMPLEXALGORITHM_H
