#include "accountmanager.h"

#include <QDebug>
#include <QStandardPaths>
#include <QFile>
#include <QVector>
#include <QMessageBox>

// TODO: need to keep track of which user logged in

SRFRS::AccountManager::AccountManager(int num) :
    m_num(num),
    dirPath(QStandardPaths::writableLocation(QStandardPaths::DataLocation)),
    dir(QDir(dirPath))
{
    // make qtSRFRS dir in AppData/Local if it doesn't exist
    if(!dir.exists()) { dir.mkpath(dirPath); }
}

bool SRFRS::AccountManager::validLogin(QString username, QString password)
{
    password = hashPassword(password);

    // get user data from .users file
    QFile userFile(dirPath + "/.users");

    if(!userFile.exists()) {

        // no users exist yet
        return false;

    } else {

        // get user data
        QVector<QString> usernames;
        QVector<QString> passwords;

        if(userFile.open(QIODevice::ReadOnly)) {
            QTextStream in(&userFile);
            while (!in.atEnd()) {
                QString line = in.readLine();

                // get the username and password
                usernames.append(line.split(" ").at(0));
                passwords.append(line.split(" ").at(1));
            }

            userFile.close();
        }

        // compare usernames and passwords with inputs
        for(int i = 0; i < usernames.size(); ++i) {
            if(usernames.at(i) == username && passwords.at(i) == password) return true;
        }

        // no matches, user input was wrong or user needs to create an account
        return false;
    }
}

bool SRFRS::AccountManager::registerUser(QString username, QString password)
{
    QFile userFile(dirPath + "/.users");

    if(!userFile.exists()) {

        // file doesn't exist, therefore no need to compare usernames
        if (userFile.open(QIODevice::WriteOnly)) {
            QTextStream stream(&userFile);

            // separate with whitespace
            stream << username + " " + hashPassword(password) << endl;

            userFile.close();
        }
    } else {

        // file does exist, need to compare usernames before registering
        QVector<QString> usernames;

        if(userFile.open(QIODevice::ReadOnly)) {
            QTextStream in(&userFile);
            while (!in.atEnd()) {
                QString line = in.readLine();

                // get the username
                line = line.split(" ").at(0);
                usernames.append(line);
            }

            userFile.close();
        }

        // check if username already exists
        for(int i = 0; i < usernames.size(); ++i) {
            if(usernames.at(i) == username) return false;
        }

        // also need to append new users, so old users are not over written
        if (userFile.open(QIODevice::ReadWrite | QIODevice::Append)) {

            // write new user to .users file
            QTextStream stream(&userFile);

            // separate with whitespace
            stream << username + " " + hashPassword(password) << endl;

            userFile.close();
        }
    }
    return true;
}

bool SRFRS::AccountManager::deleteUser(QString username)
{
    QFile userFile(dirPath + "/.users");

    // remove user from .users file
    if(userFile.open(QIODevice::ReadWrite)) {
        QString contents;
        QTextStream in(&userFile);

        while(!in.atEnd()) {
            QString line = in.readLine();
            if(!line.contains(username)) contents.append(line + "\n");
        }

        userFile.resize(0);
        in << contents;
        userFile.close();
    } else {
        return false;
    }

    return true;
}

QString SRFRS::AccountManager::hashPassword(QString password)
{
    // hash password using Sha256
    return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}
