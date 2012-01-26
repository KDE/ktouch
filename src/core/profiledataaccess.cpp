#include "profiledataaccess.h"

#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <kstandarddirs.h>
#include <kdebug.h>

ProfileDataAccess::ProfileDataAccess(QObject *parent) :
    QObject(parent)
{
    initDb();
}

void ProfileDataAccess::initDb()
{
    if (!QSqlDatabase::contains(QSqlDatabase::defaultConnection))
    {
        QString dbPath = KGlobal::dirs()->locateLocal("appdata", "profiles.db");
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbPath);
        if (!db.open())
        {
            kWarning() << db.lastError().text();
            return;
        }
        checkDbSchema();
    }
}

void ProfileDataAccess::checkDbSchema()
{
    QSqlDatabase db = QSqlDatabase::database();

    db.exec("CREATE TABLE IF NOT EXISTS metadata ("
            "key TEXT PRIMARY KEY, "
            "value TEXT"
            ")");

    QSqlQuery versionQuery = db.exec("SELECT value FROM metadata WHERE key = 'version'");

    if (versionQuery.next())
    {
        QString version = versionQuery.value(0).toString();
        if (version != "1.0")
        {
            return;
        }
    }
    else
    {
        db.exec("INSERT INTO metadata (key, value) VALUES ('version', '1.0')");
        db.commit();
    }

    db.exec("CREATE TABLE IF NOT EXISTS profiles ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT, "
            "skill_level INTEGER "
            ")");
}
