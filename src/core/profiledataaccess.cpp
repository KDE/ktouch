#include "profiledataaccess.h"

#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <kstandarddirs.h>
#include <klocale.h>
#include <kdebug.h>

#include "profile.h"

ProfileDataAccess::ProfileDataAccess(QObject *parent) :
    QObject(parent),
    m_errorMessage(QString())
{
}

void ProfileDataAccess::loadProfiles()
{
    QSqlDatabase db = database();

    if (!db.isOpen())
        return;

    qDeleteAll(m_profiles);
    m_profiles.clear();
    emit profileCountChanged();

    QSqlQuery profileQuery = db.exec("SELECT id, name, skill_level FROM profiles");

    if (db.lastError().isValid())
    {
        kWarning() << db.lastError().text();
        raiseError(db.lastError());
        return;
    }

    while(profileQuery.next())
    {
        Profile* profile = new Profile();
        profile->setId(profileQuery.value(0).toInt());
        profile->setName(profileQuery.value(1).toString());
        int rawSkillLevel = profileQuery.value(2).toInt();
        profile->setSkillLevel(rawSkillLevel == 1? Profile::Beginner: Profile::Advanced);
        m_profiles.append(profile);
    }

    emit profileCountChanged();
}

int ProfileDataAccess::profileCount() const
{
    return m_profiles.count();
}

Profile* ProfileDataAccess::profile(unsigned int index)
{
    Q_ASSERT(index < m_profiles.count());
    return m_profiles.at(index);
}

Profile* ProfileDataAccess::createProfile()
{
    return new Profile(this);
}

void ProfileDataAccess::addProfile(Profile *profile)
{
    QSqlDatabase db = database();

    if (!db.isOpen())
        return;

    if (!db.transaction())
    {
        kWarning() <<  db.lastError().text();
        raiseError(db.lastError());
        return;
    }

    QSqlQuery addQuery(db);

    if (!addQuery.prepare("INSERT INTO profiles (name, skill_level) VALUES (?, ?)"))
    {
        kWarning() <<  addQuery.lastError().text();
        raiseError(addQuery.lastError());
        return;
    }
    addQuery.bindValue(0, profile->name());
    int raw_skill_level = profile->skillLevel();
    addQuery.bindValue(1, raw_skill_level);

    if (!addQuery.exec())
    {
        kWarning() <<  addQuery.lastError().text();
        raiseError(addQuery.lastError());
        return;
    }

    if (!db.commit())
    {
        kWarning() <<  db.lastError().text();
        raiseError(db.lastError());
        db.rollback();
        return;
    }

    m_profiles.append(profile);
    emit profileCountChanged();
}

void ProfileDataAccess::updateProfile(unsigned int index)
{
    Profile* profile = this->profile(index);

    QSqlDatabase db = database();

    if (!db.isOpen())
        return;

    if (!db.transaction())
    {
        kWarning() <<  db.lastError().text();
        raiseError(db.lastError());
        return;
    }

    QSqlQuery updateQuery(db);

    if (!updateQuery.prepare("UPDATE profiles SET name = ?, skill_level = ? WHERE id = ?"))
    {
        kWarning() <<  updateQuery.lastError().text();
        raiseError(updateQuery.lastError());
        return;
    }

    updateQuery.bindValue(0, profile->name());
    int raw_skill_level = profile->skillLevel();
    updateQuery.bindValue(1, raw_skill_level);
    updateQuery.bindValue(2, profile->id());

    if (!updateQuery.exec())
    {
        kWarning() <<  updateQuery.lastError().text();
        raiseError(updateQuery.lastError());
        return;
    }

    if (!db.commit())
    {
        kWarning() <<  db.lastError().text();
        raiseError(db.lastError());
        db.rollback();
        return;
    }
}

void ProfileDataAccess::removeProfile(unsigned int index)
{
    Profile* profile = this->profile(index);

    QSqlDatabase db = database();

    if (!db.isOpen())
        return;

    if (!db.transaction())
    {
        kWarning() <<  db.lastError().text();
        raiseError(db.lastError());
        return;
    }

    QSqlQuery removeQuery(db);

    if (!removeQuery.prepare("DELETE FROM profiles WHERE id = ?"))
    {
        kWarning() <<  removeQuery.lastError().text();
        raiseError(removeQuery.lastError());
        return;
    }

    removeQuery.bindValue(0, profile->id());


    if (!removeQuery.exec())
    {
        kWarning() <<  removeQuery.lastError().text();
        raiseError(removeQuery.lastError());
        return;
    }

    if (!db.commit())
    {
        kWarning() <<  db.lastError().text();
        raiseError(db.lastError());
        db.rollback();
        return;
    }

    delete profile;
    m_profiles.removeAt(index);
}

QString ProfileDataAccess::errorMessage() const
{
    return m_errorMessage;
}

QSqlDatabase ProfileDataAccess::database()
{
    if (!QSqlDatabase::contains(QSqlDatabase::defaultConnection))
    {
        QString dbPath = KGlobal::dirs()->locateLocal("appdata", "profiles.db");
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbPath);
        if (!db.open())
        {
            kWarning() << db.lastError().text();
            raiseError(db.lastError());
            return db;
        }

        if (!checkDbSchema())
        {
            db.close();
        }

        return db;
    }

    return QSqlDatabase::database(QSqlDatabase::defaultConnection);
}

bool ProfileDataAccess::checkDbSchema()
{
    QSqlDatabase db = QSqlDatabase::database();

    db.exec("CREATE TABLE IF NOT EXISTS metadata ("
            "key TEXT PRIMARY KEY, "
            "value TEXT"
            ")");

    if (db.lastError().isValid())
    {
        kWarning() << db.lastError().text();
        raiseError(db.lastError());
        return false;
    }

    QSqlQuery versionQuery = db.exec("SELECT value FROM metadata WHERE key = 'version'");

    if (db.lastError().isValid())
    {
        kWarning() << db.lastError().text();
        raiseError(db.lastError());
        return false;
    }

    if (versionQuery.next())
    {
        QString version = versionQuery.value(0).toString();
        if (version != "1.0")
        {
            m_errorMessage = i18n("Invalid database version '%1'.").arg(version);
            emit errorMessageChanged();
            return false;
        }
    }
    else
    {
        if (!db.transaction())
        {
            kWarning() <<  db.lastError().text();
            raiseError(db.lastError());
            return false;
        }
        db.exec("INSERT INTO metadata (key, value) VALUES ('version', '1.0')");
        if (db.lastError().isValid())
        {
            kWarning() << db.lastError().text();
            raiseError(db.lastError());
            return false;
        }
        if (!db.commit())
        {
            kWarning() << db.lastError().text();
            raiseError(db.lastError());
            return false;
        }
    }

    db.exec("CREATE TABLE IF NOT EXISTS profiles ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT, "
            "skill_level INTEGER "
            ")");

    if (db.lastError().isValid())
    {
        kWarning() << db.lastError().text();
        raiseError(db.lastError());
        return false;
    }

    return true;
}

void ProfileDataAccess::raiseError(const QSqlError& error)
{
    m_errorMessage = QString("%1: %2").arg(error.driverText(), error.databaseText());
    emit errorMessageChanged();
}
