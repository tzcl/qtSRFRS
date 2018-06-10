#include "flashcardmanager.h"

#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QStringList>

#include "complexalgorithm.h"

SRFRS::FlashcardManager::FlashcardManager()
{

}

bool SRFRS::FlashcardManager::init(QString username, QString dirPath)
{
    _user = username;
    _dir = dirPath + "/" + _user + "/cards";
    QDir directory = QDir(_dir);

    // make username folder in "AppData/Local/qtSRFRS" if folder doesn't exist
    if(!directory.exists()) directory.mkpath(directory.path());

    // load flashcards
    return load();
}

bool SRFRS::FlashcardManager::load()
{
    // load decks from file
    QFile cardFile(_dir + "/.cards");

    // if file doesn't exist create it
    if(!cardFile.exists()) {
        return cardFile.open(QIODevice::WriteOnly);
    }

    if(cardFile.open(QIODevice::ReadOnly)) {
        QTextStream stream(&cardFile);
        while (!stream.atEnd()) {

            // file is formatted as: id;;deck name;;review date;;repetitions;;interval;;difficulty
            // thus split line by delimiter ";;"
            QStringList line = stream.readLine().split(";;");

            // open file containing the front of the flashcard
            QFile front(_dir + "/" + line.at(0) + ".front");
            QStringList frontText;

            if(front.open(QIODevice::ReadOnly)) {
                QTextStream stream(&front);

                while (!stream.atEnd()) {
                    // load in the front of the flashcard
                    QString text = stream.readLine();
                    frontText.append(text);
                }

                front.close();
            }

            // open file containing the back of the flashcard
            QFile back(_dir + "/" + line.at(0) + ".back");
            QStringList backText;

            if(back.open(QIODevice::ReadOnly)) {
                QTextStream stream(&back);

                while (!stream.atEnd()) {
                    // load in the back of the flashcard
                    QString text = stream.readLine();
                    backText.append(text);
                }

                back.close();
            }

            // get the flashcard data
            auto card = QSharedPointer<Flashcard>::create(line.at(0).toInt(), frontText, backText, line.at(1), QDate::fromString(line.at(2), "dd/MM/yyyy"), line.at(3).toInt(), line.at(4).toInt(), line.at(5).toFloat());

            // add to list of flashcards
            _cards.append(card);
        }

        cardFile.close();
    } else {
        // failed to load flashcards
        return false;
    }

    // successfully loaded flashcards
    return true;
}

void SRFRS::FlashcardManager::addFlashcard(QSharedPointer<Flashcard> card)
{
    // add to _cards
    _cards.append(card);

    // write to .cards
    QFile cardsFile(_dir + "/.cards");

    if (cardsFile.open(QIODevice::Append)) {

        QTextStream stream(&cardsFile);

        // write to file
        // file is formatted as: id;;deck name;;review date;;repetitions;;interval;;difficulty

        stream << QString::number(card->getID()) + ";;";
        stream << card->getDeck() + ";;";
        stream << card->getReviewDate().toString("dd/MM/yyyy") + ";;";
        stream << QString::number(card->getRepetitions()) + ";;";
        stream << QString::number(card->getInterval()) + ";;";
        stream << QString::number(card->getDifficultyCoefficient());
        stream << endl;

        cardsFile.close();
    }

    // get front and back files
    QString front_file = QString::number(card->getID()) + ".front";
    QString back_file = QString::number(card->getID()) + ".back";

    // write to front file
    QFile front(_dir + "/" + front_file);

    if(front.open(QIODevice::WriteOnly)) {

        QTextStream stream(&front);
        QStringList frontParts = card->getFront();

        // write each line of flashcard front to file
        for(int i = 0; i < frontParts.size(); ++i) {
            if(!frontParts.at(i).isEmpty()) {
                stream << frontParts.at(i) + "\n";
            }
        }

        front.close();
    }

    // write to back file
    QFile back(_dir + "/" + back_file);

    if(back.open(QIODevice::WriteOnly)) {
        QTextStream stream(&back);
        QStringList backParts = card->getBack();

        // write each line of flashcard back to file
        for(int i = 0; i < backParts.size(); ++i) {
            if(!backParts.at(i).isEmpty()) {
                stream << backParts.at(i) + "\n";
            }
        }

        back.close();
    }
}

void SRFRS::FlashcardManager::setID(int index, int oldId, int id)
{
    // set id
    _cards[index]->setID(id);

    // update .cards
    update(oldId, 0, QString::number(id));

    // update front and back files
    renameFiles(oldId, id);
}

void SRFRS::FlashcardManager::renameFiles(int oldId, int id)
{
    // get front and back files
    QFile front(_dir + "/" + QString::number(oldId) + ".front");
    QFile back(_dir + "/" + QString::number(oldId) + ".back");

    // rename front and back files
    front.rename(_dir + "/" + QString::number(id) + ".front");
    back.rename(_dir + "/" + QString::number(id) + ".back");
}

void SRFRS::FlashcardManager::removeFlashcard(int id)
{
    // get flashcard
    auto p_card = getFlashcard(id);

    // remove flashcard from memory
    if(_cards.contains(p_card)) {
        _cards.removeAll(p_card);
    } else {
        qDebug() << "_cards doesn't contain " + id;
    }

    // remove flashcardcard from file
    QFile cardsFile(_dir + "/.cards");

    if(cardsFile.open(QIODevice::ReadWrite)) {

        QString oldContents;
        QTextStream newContents(&cardsFile);

        while(!newContents.atEnd()){

            QStringList line = newContents.readLine().split(";;");

            if(line.at(0) != QString::number(id)) {
                oldContents.append(line.join(";;") + "\n");
            }
        }

        cardsFile.resize(0);
        newContents << oldContents;

        cardsFile.close();
    }

    // remove front and back files
    QFile front(_dir + "/" + QString::number(id) + ".front");
    QFile back(_dir + "/" + QString::number(id) + ".back");

    front.remove();
    back.remove();
}

QSharedPointer<SRFRS::Flashcard> SRFRS::FlashcardManager::getFlashcard(int id)
{
    // iterate through flashcards
    for(int i = 0; i < _cards.size(); ++i) {
        // find flashcard with matching ID
        if(_cards[i]->getID() == id) return _cards[i];
    }

    qDebug() << "couldn't find flashcard :S in flashcardmanager getFlashcard!! UNDEFINED BEHAVIOUR AHEAD looking for:" << QString::number(id);
    return _cards[0];
}

void SRFRS::FlashcardManager::updateFront(int id, QString after)
{
    // get front file
    QFile front(_dir + "/" + QString::number(id) + ".front");

    // get lines from the after string
    QStringList parts = after.split("\n");

    if(front.open(QIODevice::WriteOnly)) {
        QTextStream stream(&front);

        // iterate through lines in the after string
        for(int i = 0; i < parts.size(); ++i) {

            // if line is not empty write it to file
            if(parts.at(i) != "") {
                stream << parts[i] + "\n";
            }
        }

        front.close();
    }
}

void SRFRS::FlashcardManager::updateBack(int id, QString after)
{
    // get front file
    QFile back(_dir + "/" + QString::number(id) + ".back");

    // get lines from the after string
    QStringList parts = after.split("\n");

    if(back.open(QIODevice::WriteOnly)) {
        QTextStream stream(&back);

        // iterate through lines in the after string
        for(int i = 0; i < parts.size(); ++i) {

            // if line is not empty write it to file
            if(parts.at(i) != "") {
                stream << parts[i] + "\n";
            }
        }

        back.close();
    }
}

void SRFRS::FlashcardManager::schedule(int id, int difficulty)
{
    // get flashcard
    auto flashcard = getFlashcard(id);

    // get flashcard current review data
    int repetitions = flashcard->getRepetitions();
    int previousInterval = flashcard->getInterval();
    float previousDifficultyCoefficient = flashcard->getDifficultyCoefficient();

    // calculate new interval and difficulty coefficient
    float difficultyCoefficient = ComplexAlgorithm::calculateCoefficient(previousDifficultyCoefficient, difficulty);
    int newInterval = ComplexAlgorithm::schedule(repetitions, previousInterval, difficultyCoefficient);

    // calculate new review date
    QDate result = QDate::fromJulianDay(flashcard->getReviewDate().toJulianDay() + newInterval);

    // increment number of repetitions
    ++repetitions;

    // update flashcard review data
    flashcard->setRepetitions(repetitions);
    flashcard->setReviewDate(result);
    flashcard->setInterval(newInterval);
    flashcard->setDifficulty(difficultyCoefficient);

    // write new values to file
    update(id, 2, result.toString("dd/MM/yyyy"));
    update(id, 3, QString::number(repetitions));
    update(id, 4, QString::number(newInterval));
    update(id, 5, QString::number(difficultyCoefficient));
}

void SRFRS::FlashcardManager::update(int id, int index, QString after)
{
    // only 6 possible data entries: id, deck, review date, repetitions, previous interval, difficulty coefficient
    // therefore index must be between 0 and 5
    if(index < 0 || index > 5) {
        qDebug() << "index out of range!! " + QString::number(index) + " id: " + QString::number(id);
        return;
    }

    // update cards file
    QFile cardFile(_dir + "/.cards");

    if(cardFile.open(QIODevice::ReadWrite)) {

        QString existingText;
        QTextStream stream(&cardFile);

        while(!stream.atEnd()) {

            // read in new line
            QString line = stream.readLine();

            // split line by delimiter ";;"
            QStringList parts = line.split(";;");

            // find line that matches ID
            if(parts.at(0) == QString::number(id)) {
                // change component value to updated value
                parts[index] = after;
            }

            // join line by delimited ";;"
            line = parts.join(";;");

            // add line to existingText
            existingText.append(line + "\n");
        }

        // empty the file
        cardFile.resize(0);

        // rewrite in existingText plus the edited text
        stream << existingText;

        cardFile.close();
    }
}
