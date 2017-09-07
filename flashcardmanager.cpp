#include "flashcardmanager.h"

#include <QTextStream>
#include <QDebug>
#include <QStringList>
#include <QRegExp>

SRFRS::FlashcardManager::FlashcardManager()
{

}

bool SRFRS::FlashcardManager::init(QString username, QString dirPath)
{
    _user = username;
    _dirPath = dirPath + "/" + _user;
    _dir = QDir(_dirPath);

    // make username folder in "AppData/Local/qtSRFRS" if folder doesn't exist
    if(!_dir.exists()) _dir.mkpath(_dir.path());

    // load collection
    return load();
}

bool SRFRS::FlashcardManager::load()
{
    // load decks from file
    QFile cardFile(_dirPath + "/.cards");

    if(!cardFile.exists()) {
        return cardFile.open(QIODevice::WriteOnly);
    }

    if(cardFile.open(QIODevice::ReadOnly)) {
        QTextStream stream(&cardFile);
        while (!stream.atEnd()) {
            QStringList line = stream.readLine().split(";;");

            // get the flashcard data
            Flashcard card(line.at(0).toInt(), line.at(1), line.at(2), line.at(3), QDate::fromString(line.at(4), "dd/MM/yyyy"));
            _cards.append(card);
        }

        cardFile.close();
    } else {
        return false;
    }

    return true;
}

bool SRFRS::FlashcardManager::save()
{
    // save to decks file
    QFile cardFile(_dirPath + "/.cards");

    if(cardFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream stream(&cardFile);

        for(int i = 0; i < _cards.size(); ++i) {
            stream << _cards.at(i).getID() + ";;";
            stream << _cards.at(i).getFront() + ";;";
            stream << _cards.at(i).getBack() + ";;";
            stream << _cards.at(i).getDeck() + ";;";
            stream << _cards.at(i).getDate().toString("dd/MM/yyyy");
            stream << endl;
        }

        cardFile.close();
    } else {
        return false;
    }

    return true;
}

bool SRFRS::FlashcardManager::addFlashcard(Flashcard &card)
{
    _cards.append(card);

    // add to decks file
    QFile cardsFile(_dirPath + "/.cards");

    if (cardsFile.open(QIODevice::Append)) {

        QTextStream stream(&cardsFile);

        stream << QString::number(card.getID()) + ";;";
        stream << card.getFront() + ";;";
        stream << card.getBack() + ";;";
        stream << card.getDeck() + ";;";
        stream << card.getDate().toString("dd/MM/yyyy");
        stream << endl;

        cardsFile.close();
    } else {
        return false;
    }

    return true;
}

SRFRS::Flashcard SRFRS::FlashcardManager::getFlashcard(int ID)
{
    for(int i = 0; i < _cards.size(); ++i) {
        if(_cards[i].getID() == ID) {
            return _cards[i];
        }
    }

    qDebug() << "couldn't find deck :S in flashcardmanager getFlashcard";
    return _cards[0];
}

void SRFRS::FlashcardManager::setID(int index, int oldId, int id)
{
    _cards[index].setID(id);

    QFile cardFile(_dirPath + "/.cards");

    if(cardFile.open(QIODevice::ReadWrite)) {

        QString existingText;
        QTextStream stream(&cardFile);

        QRegExp regex("^(\\d+);;");

        while(!stream.atEnd()){

            QString line = stream.readLine();

            if(line.contains(regex)) {
                if(regex.cap(1) == QString::number(oldId)){
                    line.replace(regex, QString::number(id) + ";;");
                    existingText.append(line + "\n");
                } else {
                    existingText.append(line + "\n");
                }
            }
        }

        cardFile.resize(0);
        stream << existingText;

        cardFile.close();
    }
}

bool SRFRS::FlashcardManager::removeFlashcard(Flashcard &card)
{
    _cards.removeOne(card);

    // remove from decks file
    QFile cardFile(_dirPath + "/.cards");

    if(cardFile.open(QIODevice::ReadWrite)) {

        QString existingText;
        QTextStream stream(&cardFile);

        while(!stream.atEnd()){

            QString line = stream.readLine();
            QString id = line.split(";;").at(0);

            if(id != QString::number(card.getID())){
                existingText.append(line + "\n");
            }
        }

        cardFile.resize(0);
        stream << existingText;

        cardFile.close();
    } else {
        return false;
    }

    return true;
}

void SRFRS::FlashcardManager::update(int id, int index, QString after)
{
    // update cards file
    QFile cardFile(_dirPath + "/.cards");

    if(cardFile.open(QIODevice::ReadWrite)) {

        QString existingText;
        QTextStream stream(&cardFile);

        while(!stream.atEnd()){

            QString line = stream.readLine();
            QStringList parts = line.split(";;");

            if(parts.at(0) == QString::number(id)) {
                parts[index] = after;
            }

            line = parts.join(";;");

            existingText.append(line + "\n");
        }

        cardFile.resize(0);
        stream << existingText;

        cardFile.close();
    }
}
