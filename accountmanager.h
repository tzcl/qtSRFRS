#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QString>

namespace SRFRS {
class AccountManager;
}

class SRFRS::AccountManager
{
public:
    AccountManager(int num);

    /**
     * @brief validLogin: check whether the user inputs are valid.
     * @param username: the username input by the user
     * @param password: the password input by the user
     * @return: true if login successful, false otherwise
     */
    bool validLogin(QString username, QString password);

    /**
     * @brief registerUser: try to create new user, username must be unique
     * @param username: new user username, must be unique
     * @param password: new user password
     * @return: true if registered successfully, false otherwise
     */
    bool registerUser(QString username, QString password);

private:
    int m_num;
};

#endif // ACCOUNTMANAGER_H
