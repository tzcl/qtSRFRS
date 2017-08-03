#include "accountmanager.h"

#include <QDebug>

// TODO: need to keep track of which user logged in

SRFRS::AccountManager::AccountManager(int num) :
    m_num(num)
{

}

bool SRFRS::AccountManager::validLogin(QString username, QString password)
{
    qDebug() << username << password;
    qDebug() << m_num;
    return false;
}

bool SRFRS::AccountManager::registerUser(QString username, QString password)
{
    /* How this will work:
     * - see if user file exists (if not skip checks + register + write to it)
     * - get list of current users
     * - see if username already exists
     * - if it exists return false
     * - otherwise create new user, return true
     */



    return true;
}
