#include "accountmanager.h"

#include <QDebug>
#include <QFile>
#include <QVector>
#include <QMessageBox>

SRFRS::AccountManager::AccountManager(QString dirPath) :
    _dirPath(dirPath),
    _dir(QDir(dirPath))
{
    // make qtSRFRS dir in "User/AppData/Local" if directory doesn't exist
    if(!_dir.exists()) _dir.mkpath(_dirPath);
}

bool SRFRS::AccountManager::validLogin(QString username, QString password)
{
    // hash the password
    password = hashPassword(password);

    // get user data from .users file
    QFile userFile(_dirPath + "/.users");

    if(!userFile.exists()) {

        // no users exist yet, login cannot be valid
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
    QFile userFile(_dirPath + "/.users");

    if(!userFile.exists()) {
        // no other users exist yet, can't compare username to anything

        // append user to .users file
        if (userFile.open(QIODevice::WriteOnly)) {
            QTextStream stream(&userFile);

            // separate username and password with whitespace
            stream << username + " " + hashPassword(password) << endl;

            userFile.close();
        }
    } else {

        // file does exist, need to compare username before registering
        QVector<QString> usernames;

        if(userFile.open(QIODevice::ReadOnly)) {
            QTextStream in(&userFile);
            while (!in.atEnd()) {
                QString line = in.readLine();

                // get the existing user's username
                line = line.split(" ").at(0);
                usernames.append(line);
            }

            userFile.close();
        }

        // check if username already exists
        for(int i = 0; i < usernames.size(); ++i) {
            if(usernames.at(i) == username) return false;
        }

        // append new user
        if (userFile.open(QIODevice::ReadWrite | QIODevice::Append)) {

            // write new user to .users file
            QTextStream stream(&userFile);

            // separate username and password with whitespace
            stream << username + " " + hashPassword(password) << endl;

            userFile.close();
        }
    }
    return true;
}

bool SRFRS::AccountManager::deleteUser(QString username)
{
    // remove user from .users file
    QFile userFile(_dirPath + "/.users");

    if(userFile.open(QIODevice::ReadWrite)) {
        QString contents;
        QTextStream in(&userFile);

        while(!in.atEnd()) {
            // read line
            QString line = in.readLine();

            // check if line contains username to delete
            if(!line.contains(username)) {
                // if line doesn't contain username, append it to contents
                contents.append(line + "\n");
            } // else ignore line
        }

        // empty file
        userFile.resize(0);

        // stream in contents
        in << contents;
        userFile.close();
    } else {
        return false;
    }

    // remove user folder and subfolders
    QDir userDir(_dirPath + "/" + username);

    if(userDir.exists()) {
        userDir.removeRecursively();
    }

    return true;
} 
