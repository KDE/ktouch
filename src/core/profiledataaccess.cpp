/*
 *  Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "profiledataaccess.h"

#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <kstandarddirs.h>
#include <klocale.h>
#include <kdebug.h>

#include "profile.h"
#include "trainingstats.h"

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

    foreach(Profile* profile, m_profiles)
        profile->deleteLater();

    m_profiles.clear();
    emit profileCountChanged();

    QSqlQuery profileQuery = db.exec("SELECT id, name, skill_level, last_used_course_id FROM profiles");

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
        profile->setLastUsedCourseId(profileQuery.value(3).toString());
        m_profiles.append(profile);
    }

    emit profileCountChanged();
}

int ProfileDataAccess::profileCount() const
{
    return m_profiles.count();
}

Profile* ProfileDataAccess::profile(int index)
{
    Q_ASSERT(index >= 0 && index < m_profiles.count());
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

    if (!addQuery.prepare("INSERT INTO profiles (name, skill_level, last_used_course_id) VALUES (?, ?, ?)"))
    {
        kWarning() <<  addQuery.lastError().text();
        raiseError(addQuery.lastError());
        return;
    }
    addQuery.bindValue(0, profile->name());
    addQuery.bindValue(1, profile->skillLevel());
    addQuery.bindValue(2, profile->lastUsedCourseId());

    if (!addQuery.exec())
    {
        kWarning() <<  addQuery.lastError().text();
        raiseError(addQuery.lastError());
        return;
    }

    QSqlQuery idQuery = db.exec("SELECT last_insert_rowid()");

    if (db.lastError().isValid())
    {
        kWarning() << db.lastError().text();
        raiseError(db.lastError());
        return;
    }

    idQuery.next();
    profile->setId(idQuery.value(0).toInt());

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

void ProfileDataAccess::updateProfile(int index)
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

    if (!updateQuery.prepare("UPDATE profiles SET name = ?, skill_level = ?, last_used_course_id = ? WHERE id = ?"))
    {
        kWarning() <<  updateQuery.lastError().text();
        raiseError(updateQuery.lastError());
        return;
    }

    updateQuery.bindValue(0, profile->name());
    updateQuery.bindValue(1, profile->skillLevel());
    updateQuery.bindValue(2, profile->lastUsedCourseId());
    updateQuery.bindValue(3, profile->id());

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

void ProfileDataAccess::removeProfile(int index)
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

    m_profiles.removeAt(index);
    emit profileCountChanged();
    profile->deleteLater();
}

int ProfileDataAccess::indexOfProfile(Profile *profile)
{
    for (int i = 0; i < m_profiles.length(); i++) {
        if (profile == m_profiles.at(i)) {
            return i;
        }
    }
    return -1;
}

void ProfileDataAccess::loadReferenceTrainingStats(TrainingStats *stats, Profile *profile, const QString &courseId, const QString &lessonId)
{
    stats->setCharactersTyped(0);
    stats->setEllapsedTime(QTime());
    stats->setErrorCount(0);
    stats->setErrorMap(QMap<QString, int>());
    stats->setIsValid(false);

    QSqlDatabase db = database();

    if (!db.isOpen())
        return;

    QSqlQuery selectQuery;

    if (!selectQuery.prepare("SELECT id, characters_typed, error_count, ellapsed_time FROM training_stats WHERE profile_id = ? AND course_id = ? AND lesson_id = ? ORDER BY date DESC LIMIT 1"))
    {
        kWarning() <<  selectQuery.lastError().text();
        raiseError(selectQuery.lastError());
        return;
    }

    selectQuery.bindValue(0, profile->id());
    selectQuery.bindValue(1, courseId);
    selectQuery.bindValue(2, lessonId);

    if (!selectQuery.exec())
    {
        kWarning() <<  selectQuery.lastError().text();
        raiseError(selectQuery.lastError());
        return;
    }

    if (!selectQuery.next())
        return;

    const int statsId = selectQuery.value(0).toInt();
    stats->setCharactersTyped(selectQuery.value(1).toUInt());
    stats->setErrorCount(selectQuery.value(2).toUInt());
    stats->setEllapsedTime(selectQuery.value(3).toInt());

    QSqlQuery errorSelectQuery;

    errorSelectQuery.prepare("SELECT character, count FROM training_stats_errors WHERE stats_id = ?");

    errorSelectQuery.bindValue(0, statsId);

    if (!errorSelectQuery.exec())
    {
        kWarning() <<  errorSelectQuery.lastError().text();
        raiseError(errorSelectQuery.lastError());
        return;
    }

    QMap<QString, int> errorMap;

    while (errorSelectQuery.next())
    {
        const QString character = errorSelectQuery.value(0).toString();
        const int errorCount = errorSelectQuery.value(1).toInt();
        errorMap.insert(character, errorCount);
    }

    stats->setErrorMap(errorMap);
    stats->setIsValid(true);
}

void ProfileDataAccess::saveTrainingStats(TrainingStats *stats, Profile *profile, const QString &courseId, const QString &lessonId)
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

    if (!addQuery.prepare("INSERT INTO training_stats (profile_id, course_id, lesson_id, date, characters_typed, error_count, ellapsed_time) VALUES (?, ?, ?, ?, ?, ?, ?)"))
    {
        kWarning() <<  addQuery.lastError().text();
        raiseError(addQuery.lastError());
        return;
    }
    addQuery.bindValue(0, profile->id());
    addQuery.bindValue(1, courseId);
    addQuery.bindValue(2, lessonId);
    addQuery.bindValue(3, QDateTime::currentMSecsSinceEpoch());
    addQuery.bindValue(4, stats->charactesTyped());
    addQuery.bindValue(5, stats->errorCount());
    const int rawEllapsedTime = QTime(0, 0).msecsTo(stats->ellapsedTime());
    addQuery.bindValue(6, rawEllapsedTime);

    if (!addQuery.exec())
    {
        kWarning() <<  addQuery.lastError().text();
        raiseError(addQuery.lastError());
        return;
    }

    QSqlQuery idQuery = db.exec("SELECT last_insert_rowid()");

    if (db.lastError().isValid())
    {
        kWarning() << db.lastError().text();
        raiseError(db.lastError());
        return;
    }

    idQuery.next();
    int statsId = idQuery.value(0).toInt();

    QSqlQuery addErrorsQuery(db);

    if (!addErrorsQuery.prepare("INSERT INTO training_stats_errors (stats_id, character, count) VALUES (?, ?, ?)"))
    {
        kWarning() <<  addErrorsQuery.lastError().text();
        raiseError(addErrorsQuery.lastError());
        return;
    }

    QMapIterator<QString, int> errorIterator(stats->errorMap());
    while(errorIterator.hasNext())
    {
        errorIterator.next();
        addErrorsQuery.bindValue(0, statsId);
        addErrorsQuery.bindValue(1, errorIterator.key());
        addErrorsQuery.bindValue(2, errorIterator.value());

        if (!addErrorsQuery.exec())
        {
            kWarning() <<  addErrorsQuery.lastError().text();
            raiseError(addErrorsQuery.lastError());
            return;
        }
    }

    if(!db.commit())
    {
        kWarning() <<  db.lastError().text();
        raiseError(db.lastError());
        return;
    }
}

QString ProfileDataAccess::courseProgress(Profile* profile, const QString& courseId, CourseProgressType type)
{
    bool idOk;
    int id = findCourseProgressId(profile, courseId, type, &idOk);

    if (!idOk || id == -1)
        return QString();

    QSqlDatabase db = database();

    if (!db.isOpen())
        return QString();

    QSqlQuery selectQuery;

    selectQuery.prepare("SELECT lesson_id FROM course_progress WHERE id = ?");

    selectQuery.bindValue(0, id);

    if (!selectQuery.exec())
    {
        kWarning() <<  selectQuery.lastError().text();
        raiseError(selectQuery.lastError());
        return QString();
    }

    selectQuery.next();
    return selectQuery.value(0).toString();
}

void ProfileDataAccess::saveCourseProgress(const QString& lessonId, Profile* profile, const QString& courseId, CourseProgressType type)
{
    bool idOk;
    int id = findCourseProgressId(profile, courseId, type, &idOk);

    if (!idOk)
        return;

    QSqlDatabase db = database();

    if (!db.isOpen())
        return;

    if (!db.transaction())
    {
        kWarning() <<  db.lastError().text();
        raiseError(db.lastError());
        return;
    }

    if (id == -1)
    {
        QSqlQuery insertQuery;

        insertQuery.prepare("INSERT INTO course_progress (profile_id, course_id, type, lesson_id) VALUES (?, ?, ?, ?)");

        insertQuery.bindValue(0, profile->id());
        insertQuery.bindValue(1, courseId);
        insertQuery.bindValue(2, type);
        insertQuery.bindValue(3, lessonId);

        if (!insertQuery.exec())
        {
            kWarning() <<  insertQuery.lastError().text();
            raiseError(insertQuery.lastError());
            return;
        }
    }
    else
    {
        QSqlQuery updateQuery;

        updateQuery.prepare("UPDATE course_progress SET lesson_id = ? WHERE id = ?");

        updateQuery.bindValue(0, lessonId);
        updateQuery.bindValue(1, id);

        if (!updateQuery.exec())
        {
            kWarning() <<  updateQuery.lastError().text();
            raiseError(updateQuery.lastError());
            return;
        }
    }

    if(!db.commit())
    {
        kWarning() <<  db.lastError().text();
        raiseError(db.lastError());
        return;
    }
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

    db.exec("CREATE TABLE IF NOT EXISTS training_stats ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "profile_id INTEGER, "
            "course_id TEXT, "
            "lesson_id TEXT, "
            "date INT, "
            "characters_typed INTEGER, "
            "error_count INTEGER, "
            "ellapsed_time INTEGER "
            ")");

    if (db.lastError().isValid())
    {
        kWarning() << db.lastError().text();
        raiseError(db.lastError());
        return false;
    }

    db.exec("CREATE TABLE IF NOT EXISTS training_stats_errors ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "stats_id INTEGER, "
            "character TEXT, "
            "count INTEGER "
            ")");

    if (db.lastError().isValid())
    {
        kWarning() << db.lastError().text();
        raiseError(db.lastError());
        return false;
    }

    db.exec("CREATE TABLE IF NOT EXISTS course_progress ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "profile_id INTEGER, "
            "course_id TEXT, "
            "type INTEGER, "
            "lesson_id TEXT "
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

int ProfileDataAccess::findCourseProgressId(Profile* profile, const QString& courseId, CourseProgressType type, bool* ok)
{
    *ok = false;

    QSqlDatabase db = database();

    if (!db.isOpen())
        return -1;

    QSqlQuery findQuery;

    findQuery.prepare("SELECT id FROM course_progress WHERE profile_id = ? AND course_id = ? AND type = ? LIMIT 1");

    findQuery.bindValue(0, profile->id());
    findQuery.bindValue(1, courseId);
    findQuery.bindValue(2, type);

    if (!findQuery.exec())
    {
        kWarning() << findQuery.lastError().text();
        raiseError(findQuery.lastError());
        return -1;
    }

    *ok = true;

    if (!findQuery.next())
    {
        return -1;
    }

    return findQuery.value(0).toInt();
}
