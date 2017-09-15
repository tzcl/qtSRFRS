#include "flashcardmanager.h"

#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QStringList>

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

    if(!cardFile.exists()) {
        return cardFile.open(QIODevice::WriteOnly);
    }

    if(cardFile.open(QIODevice::ReadOnly)) {
        QTextStream stream(&cardFile);
        while (!stream.atEnd()) {
            QStringList line = stream.readLine().split(";;");

            QFile front(_dir + "/" + line.at(0) + ".front");
            QStringList frontText;

            if(front.open(QIODevice::ReadOnly)) {
                QTextStream stream(&front);

                while (!stream.atEnd()) {
                    QString text = stream.readLine();
                    frontText.append(text);
                }

                front.close();
            }

            QFile back(_dir + "/" + line.at(0) + ".back");
            QStringList backText;

            if(back.open(QIODevice::ReadOnly)) {
                QTextStream stream(&back);

                while (!stream.atEnd()) {
                    QString text = stream.readLine();
                    backText.append(text);
                }

                back.close();
            }

            // get the flashcard data
            auto card = QSharedPointer<Flashcard>::create(line.at(0).toInt(), frontText, backText, line.at(1), QDate::fromString(line.at(2), "dd/MM/yyyy"));
            _cards.append(card);
        }

        cardFile.close();
    } else {
        return false;
    }

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

        stream << QString::number(card->getID()) + ";;";
        stream << card->getDeck() + ";;";
        stream << card->getDate().toString("dd/MM/yyyy");
        stream << endl;

        cardsFile.close();
    }

    QString front_file = QString::number(card->getID()) + ".front";
    QString back_file = QString::number(card->getID()) + ".back";

    // write to front file
    QFile front(_dir + "/" + front_file);

    if(front.open(QIODevice::WriteOnly)) {

        QTextStream stream(&front);
        QStringList frontParts = card->getFront();

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
    QFile front(_dir + "/" + QString::number(oldId) + ".front");
    QFile back(_dir + "/" + QString::number(oldId) + ".back");

    front.rename(_dir + "/" + QString::number(id) + ".front");
    back.rename(_dir + "/" + QString::number(id) + ".back");
}

void SRFRS::FlashcardManager::removeFlashcard(int id)
{
    auto p_card = getFlashcard(id);

    // remove card from _cards
    if(_cards.contains(p_card)) {
        _cards.removeAll(p_card);
    } else {
        qDebug() << "_cards doesn't contain " + id;
    }

    // remove card from .cards
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
    for(int i = 0; i < _cards.size(); ++i) {
        if(_cards[i]->getID() == id) return _cards[i];
    }

    qDebug() << "couldn't find flashcard :S in flashcardmanager getFlashcard!! UNDEFINED BEHAVIOUR AHEAD";
    return _cards[0];
}

void SRFRS::FlashcardManager::updateFront(int id, QString after)
{
    QFile front(_dir + "/" + QString::number(id) + ".front");

    QStringList parts = after.split("\n");

    if(front.open(QIODevice::WriteOnly)) {
        QTextStream stream(&front);

        for(int i = 0; i < parts.size(); ++i) {
            if(parts.at(i) != "") {
                stream << parts[i] + "\n";
            }
        }

        front.close();
    }
}

void SRFRS::FlashcardManager::updateBack(int id, QString after)
{
    QFile back(_dir + "/" + QString::number(id) + ".back");

    QStringList parts = after.split("\n");

    if(back.open(QIODevice::WriteOnly)) {
        QTextStream stream(&back);

        for(int i = 0; i < parts.size(); ++i) {
            if(parts.at(i) != "") {
                stream << parts[i] + "\n";
            }
        }

        back.close();
    }
}

void SRFRS::FlashcardManager::update(int id, int index, QString after)
{
    // only 3 possible data entries: id, deck, date created
    // therefore index must be between 0 and 2
    if(index < 0 || index > 2) {
        qDebug() << "index out of range!! " + QString::number(index) + " id: " + QString::number(id);
        return;
    }

    // TODO: update front/back files

    // update cards file
    QFile deckFile(_dir + "/.cards");

    if(deckFile.open(QIODevice::ReadWrite)) {

        QString existingText;
        QTextStream stream(&deckFile);

        while(!stream.atEnd()){

            QString line = stream.readLine();
            QStringList parts = line.split(";;");

            if(parts.at(0) == QString::number(id)) {
                parts[index] = after;
            }

            line = parts.join(";;");

            existingText.append(line + "\n");
        }

        deckFile.resize(0);
        stream << existingText;

        deckFile.close();
    }
}
