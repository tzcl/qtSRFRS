#include "accountmanager.h"

#include <QDebug>
#include <QStandardPaths>
#include <QFile>

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
    qDebug() << username << password;
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

    QFile userFile(dirPath + "/.users");
    qDebug() << userFile.exists();

    if(!userFile.exists()) {
        if (userFile.open(QIODevice::ReadWrite)) {
            QTextStream stream(&userFile);
            stream << "something" << endl;
        }
    }

    qDebug() << userFile.exists();

    return true;
}
