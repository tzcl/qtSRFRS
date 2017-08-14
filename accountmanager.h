#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QString>
#include <QDir>
#include <QCryptographicHash>

#include "deckmanager.h"

namespace SRFRS {

class AccountManager
{
public:
    AccountManager();

    /**
     * @brief validLogin: Check whether the user inputs are valid.
     * @param username: the username input by the user
     * @param password: the password input by the user
     * @return: True if matching login details exist in the .users file and
     *          the .users file exists, false otherwise.
     */
    bool validLogin(QString username, QString password);

    /**
     * @brief registerUser: Try to create new user, username must be unique.
     *                      Assumes that inputs have been validated.
     *                      Writes the new user details to the .users file.
     * @param username: new user username, must be unique
     * @param password: new user password
     * @return: False if username already exists, true otherwise.
     */
    bool registerUser(QString username, QString password);

    /**
     * @brief deleteUser: Try to delete the current user.
     * @param username: The username of the current user.
     * @return: True if the user was deleted successfully, false otherwise.
     */
    bool deleteUser(QString username);

private:

    /**
     * @brief hashPassword: Hash the password using SHA256
     * @param password: The user's password
     * @return: The hash generated by the password
     */
    QString hashPassword(QString password) {
        return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
    }

    QString _dirPath;
    QDir _dir;
};

}

#endif // ACCOUNTMANAGER_H
