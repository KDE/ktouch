/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "profiledataaccess.h"

#include <QDebug>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlError>

#include <KLocalizedString>

#include "core/profile.h"
#include "core/course.h"
#include "core/lesson.h"
#include "core/keyboardlayout.h"
#include "core/trainingstats.h"

ProfileDataAccess::ProfileDataAccess(QObject* parent) :
    DbAccess(parent)
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

    QSqlQuery profileQuery = db.exec(QStringLiteral("SELECT id, name, skill_level, last_used_course_id FROM profiles"));

    if (db.lastError().isValid())
    {
        qWarning() << db.lastError().text();
        raiseError(db.lastError());
        return;
    }

    while(profileQuery.next())
    {
        Profile* profile = new Profile(this);
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

void ProfileDataAccess::addProfile(Profile* profile)
{
    QSqlDatabase db = database();

    if (!db.isOpen())
        return;

    if (!db.transaction())
    {
        qWarning() <<  db.lastError().text();
        raiseError(db.lastError());
        return;
    }

    QSqlQuery addQuery(db);

    if (!addQuery.prepare(QStringLiteral("INSERT INTO profiles (name, skill_level, last_used_course_id) VALUES (?, ?, ?)")))
    {
        qWarning() <<  addQuery.lastError().text();
        raiseError(addQuery.lastError());
        db.rollback();
        return;
    }
    addQuery.bindValue(0, profile->name());
    addQuery.bindValue(1, profile->skillLevel());
    addQuery.bindValue(2, profile->lastUsedCourseId());

    if (!addQuery.exec())
    {
        qWarning() <<  addQuery.lastError().text();
        raiseError(addQuery.lastError());
        db.rollback();
        return;
    }

    QSqlQuery idQuery = db.exec(QStringLiteral("SELECT last_insert_rowid()"));

    if (db.lastError().isValid())
    {
        qWarning() << db.lastError().text();
        raiseError(db.lastError());
        db.rollback();
        return;
    }

    idQuery.next();
    profile->setId(idQuery.value(0).toInt());

    if (!db.commit())
    {
        qWarning() <<  db.lastError().text();
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
        qWarning() <<  db.lastError().text();
        raiseError(db.lastError());
        return;
    }

    QSqlQuery updateQuery(db);

    if (!updateQuery.prepare(QStringLiteral("UPDATE profiles SET name = ?, skill_level = ?, last_used_course_id = ? WHERE id = ?")))
    {
        qWarning() <<  updateQuery.lastError().text();
        raiseError(updateQuery.lastError());
        return;
    }

    updateQuery.bindValue(0, profile->name());
    updateQuery.bindValue(1, profile->skillLevel());
    updateQuery.bindValue(2, profile->lastUsedCourseId());
    updateQuery.bindValue(3, profile->id());

    if (!updateQuery.exec())
    {
        qWarning() <<  updateQuery.lastError().text();
        raiseError(updateQuery.lastError());
        db.rollback();
        return;
    }

    if (!db.commit())
    {
        qWarning() <<  db.lastError().text();
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
        qWarning() <<  db.lastError().text();
        raiseError(db.lastError());
        db.rollback();
        return;
    }

    QSqlQuery removeQuery(db);

    if (!removeQuery.prepare(QStringLiteral("DELETE FROM profiles WHERE id = ?")))
    {
        qWarning() <<  removeQuery.lastError().text();
        raiseError(removeQuery.lastError());
        db.rollback();
        return;
    }

    removeQuery.bindValue(0, profile->id());


    if (!removeQuery.exec())
    {
        qWarning() <<  removeQuery.lastError().text();
        raiseError(removeQuery.lastError());
        db.rollback();
        return;
    }

    if (!db.commit())
    {
        qWarning() <<  db.lastError().text();
        raiseError(db.lastError());
        db.rollback();
        return;
    }

    m_profiles.removeAt(index);
    emit profileCountChanged();
    profile->deleteLater();
}

int ProfileDataAccess::indexOfProfile(Profile* profile)
{
    for (int i = 0; i < m_profiles.length(); i++) {
        if (profile == m_profiles.at(i)) {
            return i;
        }
    }
    return -1;
}

void ProfileDataAccess::loadReferenceTrainingStats(TrainingStats* stats, Profile* profile, const QString& courseId, const QString& lessonId)
{
    stats->setCharactersTyped(0);
    stats->setElapsedTime(QTime());
    stats->setErrorCount(0);
    stats->setErrorMap(QMap<QString, int>());
    stats->setIsValid(false);

    QSqlDatabase db = database();

    if (!db.isOpen())
        return;

    QSqlQuery selectQuery;

    if (!selectQuery.prepare(QStringLiteral("SELECT id, characters_typed, error_count, elapsed_time FROM training_stats WHERE profile_id = ? AND course_id = ? AND lesson_id = ? ORDER BY date DESC LIMIT 1")))
    {
        qWarning() <<  selectQuery.lastError().text();
        raiseError(selectQuery.lastError());
        return;
    }

    selectQuery.bindValue(0, profile->id());
    selectQuery.bindValue(1, courseId);
    selectQuery.bindValue(2, lessonId);

    if (!selectQuery.exec())
    {
        qWarning() <<  selectQuery.lastError().text();
        raiseError(selectQuery.lastError());
        return;
    }

    if (!selectQuery.next())
        return;

    const int statsId = selectQuery.value(0).toInt();
    stats->setCharactersTyped(selectQuery.value(1).toUInt());
    stats->setErrorCount(selectQuery.value(2).toUInt());
    stats->setElapsedTime(selectQuery.value(3).toInt());

    QSqlQuery errorSelectQuery;

    errorSelectQuery.prepare(QStringLiteral("SELECT character, count FROM training_stats_errors WHERE stats_id = ?"));

    errorSelectQuery.bindValue(0, statsId);

    if (!errorSelectQuery.exec())
    {
        qWarning() <<  errorSelectQuery.lastError().text();
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

void ProfileDataAccess::saveTrainingStats(TrainingStats* stats, Profile* profile, const QString& courseId, const QString& lessonId)
{
    QSqlDatabase db = database();

    if (!db.isOpen())
        return;

    if (!db.transaction())
    {
        qWarning() <<  db.lastError().text();
        raiseError(db.lastError());
        return;
    }
    QSqlQuery addQuery(db);

    if (!addQuery.prepare(QStringLiteral("INSERT INTO training_stats (profile_id, course_id, lesson_id, date, characters_typed, error_count, elapsed_time) VALUES (?, ?, ?, ?, ?, ?, ?)")))
    {
        qWarning() <<  addQuery.lastError().text();
        raiseError(addQuery.lastError());
        db.rollback();
        return;
    }
    addQuery.bindValue(0, profile->id());
    addQuery.bindValue(1, courseId);
    addQuery.bindValue(2, lessonId);
    addQuery.bindValue(3, QDateTime::currentMSecsSinceEpoch());
    addQuery.bindValue(4, stats->charactesTyped());
    addQuery.bindValue(5, stats->errorCount());
    const int rawElapsedTime = QTime(0, 0).msecsTo(stats->elapsedTime());
    addQuery.bindValue(6, rawElapsedTime);

    if (!addQuery.exec())
    {
        qWarning() <<  addQuery.lastError().text();
        raiseError(addQuery.lastError());
        db.rollback();
        return;
    }

    QSqlQuery idQuery = db.exec(QStringLiteral("SELECT last_insert_rowid()"));

    if (db.lastError().isValid())
    {
        qWarning() << db.lastError().text();
        raiseError(db.lastError());
        db.rollback();
        return;
    }

    idQuery.next();
    int statsId = idQuery.value(0).toInt();

    QSqlQuery addErrorsQuery(db);

    if (!addErrorsQuery.prepare(QStringLiteral("INSERT INTO training_stats_errors (stats_id, character, count) VALUES (?, ?, ?)")))
    {
        qWarning() <<  addErrorsQuery.lastError().text();
        raiseError(addErrorsQuery.lastError());
        db.rollback();
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
            qWarning() <<  addErrorsQuery.lastError().text();
            raiseError(addErrorsQuery.lastError());
            db.rollback();
            return;
        }
    }

    if(!db.commit())
    {
        qWarning() <<  db.lastError().text();
        raiseError(db.lastError());
        db.rollback();
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

    selectQuery.prepare(QStringLiteral("SELECT lesson_id FROM course_progress WHERE id = ?"));

    selectQuery.bindValue(0, id);

    if (!selectQuery.exec())
    {
        qWarning() <<  selectQuery.lastError().text();
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
        qWarning() <<  db.lastError().text();
        raiseError(db.lastError());
        return;
    }

    if (id == -1)
    {
        QSqlQuery insertQuery;

        insertQuery.prepare(QStringLiteral("INSERT INTO course_progress (profile_id, course_id, type, lesson_id) VALUES (?, ?, ?, ?)"));

        insertQuery.bindValue(0, profile->id());
        insertQuery.bindValue(1, courseId);
        insertQuery.bindValue(2, type);
        insertQuery.bindValue(3, lessonId);

        if (!insertQuery.exec())
        {
            qWarning() <<  insertQuery.lastError().text();
            raiseError(insertQuery.lastError());
            db.rollback();
            return;
        }
    }
    else
    {
        QSqlQuery updateQuery;

        updateQuery.prepare(QStringLiteral("UPDATE course_progress SET lesson_id = ? WHERE id = ?"));

        updateQuery.bindValue(0, lessonId);
        updateQuery.bindValue(1, id);

        if (!updateQuery.exec())
        {
            qWarning() <<  updateQuery.lastError().text();
            raiseError(updateQuery.lastError());
            db.rollback();
            return;
        }
    }

    if(!db.commit())
    {
        qWarning() <<  db.lastError().text();
        raiseError(db.lastError());
        db.rollback();
        return;
    }
}

int ProfileDataAccess::lessonsTrained(Profile* profile)
{
    QSqlDatabase db = database();

    if (!profile)
        return 0;

    QString sql = QStringLiteral("SELECT COUNT(*) FROM training_stats WHERE profile_id = ?");

    QSqlQuery query(db);

    query.prepare(sql);
    query.bindValue(0, profile->id());

    if (!query.exec())
    {
        qWarning() <<  query.lastError().text();
        raiseError(query.lastError());
        return 0;
    }

    query.next();
    return query.value(0).toInt();
}

quint64 ProfileDataAccess::totalTrainingTime(Profile* profile)
{
    QSqlDatabase db = database();

    if (!profile)
        return 0;

    QString sql = QStringLiteral("SELECT SUM(elapsed_time) FROM training_stats WHERE profile_id = ?");

    QSqlQuery query(db);

    query.prepare(sql);
    query.bindValue(0, profile->id());

    if (!query.exec())
    {
        qWarning() <<  query.lastError().text();
        raiseError(query.lastError());
        return 0;
    }

    query.next();
    return query.value(0).value<quint64>();
}

QDateTime ProfileDataAccess::lastTrainingSession(Profile* profile)
{
    QSqlDatabase db = database();

    if (!profile)
        return QDateTime();

    QString sql = QStringLiteral("SELECT date FROM training_stats WHERE profile_id = ? ORDER BY date DESC LIMIT 1");

    QSqlQuery query(db);

    query.prepare(sql);
    query.bindValue(0, profile->id());

    if (!query.exec())
    {
        qWarning() <<  query.lastError().text();
        raiseError(query.lastError());
        return QDateTime();
    }

    if (!query.next())
        return QDateTime();

    return QDateTime::fromMSecsSinceEpoch(query.value(0).value<quint64>());
}

bool ProfileDataAccess::loadCustomLessons(Profile* profile, const QString& keyboardLayoutNameFilter, Course* target)
{
    target->setIsValid(false);

    QSqlDatabase db = database();

    if (!profile)
        return false;

    if (!db.isOpen())
        return false;

    QString sql = QStringLiteral("SELECT id, title, text, keyboard_layout_name FROM custom_lessons WHERE profile_id = ?");

    if (!keyboardLayoutNameFilter.isNull())
    {
        sql += QLatin1String(" AND keyboard_layout_name = ?");
    }

    QSqlQuery query(db);

    query.prepare(sql);

    query.bindValue(0, profile->id());

    if (!keyboardLayoutNameFilter.isNull())
    {
        query.bindValue(1, keyboardLayoutNameFilter);
    }

    if (!query.exec())
    {
        qWarning() <<  query.lastError().text();
        raiseError(query.lastError());
        return false;
    }

    if (!query.isActive())
        return false;

    target->setKind(Course::LessonCollection);
    target->setId(QStringLiteral("custom_lessons"));
    target->setTitle(i18n("Custom Lessons"));
    target->setDescription(i18n("A place to store personal lesson texts"));
    target->setKeyboardLayoutName(keyboardLayoutNameFilter);
    target->clearLessons();

    while (query.next())
    {
        Lesson* lesson = new Lesson(this);

        lesson->setId(query.value(0).toString());
        lesson->setTitle(query.value(1).toString());

        const QString text = query.value(2).toString();
        QString characters = QLatin1String("");

        for (int i = 0; i < text.length(); i++)
        {
            const QChar character = text.at(i);

            if (!characters.contains(character))
            {
                characters.append(character);
            }
        }

        lesson->setText(text);
        lesson->setCharacters(characters);

        target->addLesson(lesson);
    }

    target->setIsValid(true);
    return true;
}

bool ProfileDataAccess::storeCustomLesson(Lesson* lesson, Profile* profile, const QString& keyboardLayoutName)
{
    QSqlDatabase db = database();

    if (!db.isOpen())
        return false;

    if (!db.transaction())
    {
        qWarning() <<  db.lastError().text();
        raiseError(db.lastError());
        return false;
    }

    QSqlQuery idQuery(db);

    idQuery.prepare(QStringLiteral("SELECT count(*) FROM custom_lessons WHERE id = ?"));
    idQuery.bindValue(0, lesson->id());
    idQuery.exec();

    if (idQuery.lastError().isValid())
    {
        qWarning() << idQuery.lastError().text();
        raiseError(idQuery.lastError());
        db.rollback();
        return false;
    }

    idQuery.next();
    const bool lessonAlreadyExists = idQuery.value(0).toInt() == 1;

    if (lessonAlreadyExists)
    {
        QSqlQuery updateQuery(db);

        updateQuery.prepare(QStringLiteral("UPDATE custom_lessons SET profile_id = ?, title = ?, text = ?, keyboard_layout_name = ? WHERE id = ?"));

        if (updateQuery.lastError().isValid())
        {
            qWarning() << updateQuery.lastError().text();
            raiseError(updateQuery.lastError());
            db.rollback();
            return false;
        }

        updateQuery.bindValue(0, profile->id());
        updateQuery.bindValue(1, lesson->title());
        updateQuery.bindValue(2, lesson->text());
        updateQuery.bindValue(3, keyboardLayoutName);
        updateQuery.bindValue(4, lesson->id());

        updateQuery.exec();

        if (updateQuery.lastError().isValid())
        {
            qWarning() << updateQuery.lastError().text();
            raiseError(updateQuery.lastError());
            db.rollback();
            return false;
        }
    }
    else
    {
        QSqlQuery insertQuery(db);

        insertQuery.prepare(QStringLiteral("INSERT INTO custom_lessons (id, profile_id, title, text, keyboard_layout_name) VALUES (?, ?, ?, ?, ?)"));

        if (insertQuery.lastError().isValid())
        {
            qWarning() << insertQuery.lastError().text();
            raiseError(insertQuery.lastError());
            db.rollback();
            return false;
        }

        insertQuery.bindValue(0, lesson->id());
        insertQuery.bindValue(1, profile->id());
        insertQuery.bindValue(2, lesson->title());
        insertQuery.bindValue(3, lesson->text());
        insertQuery.bindValue(4, keyboardLayoutName);

        insertQuery.exec();

        if (insertQuery.lastError().isValid())
        {
            qWarning() << insertQuery.lastError().text();
            raiseError(insertQuery.lastError());
            db.rollback();
            return false;
        }
    }

    if(!db.commit())
    {
        qWarning() <<  db.lastError().text();
        raiseError(db.lastError());
        db.rollback();
        return false;
    }

    return true;
}

bool ProfileDataAccess::deleteCustomLesson(const QString& id)
{
    QSqlDatabase db = database();

    if (!db.isOpen())
        return false;

    if (!db.transaction())
    {
        qWarning() <<  db.lastError().text();
        raiseError(db.lastError());
        return false;
    }

    QSqlQuery deleteQuery(db);

    deleteQuery.prepare(QStringLiteral("DELETE FROM custom_lessons WHERE id = ?"));
    deleteQuery.bindValue(0, id);
    deleteQuery.exec();

    if (deleteQuery.lastError().isValid())
    {
        qWarning() << deleteQuery.lastError().text();
        raiseError(deleteQuery.lastError());
        db.rollback();
        return false;
    }

    if(!db.commit())
    {
        qWarning() <<  db.lastError().text();
        raiseError(db.lastError());
        db.rollback();
        return false;
    }

    return true;
}

QSqlQuery ProfileDataAccess::learningProgressQuery(Profile* profile, Course* courseFilter, Lesson* lessonFilter)
{
    QSqlDatabase db = database();

    if (!profile)
        return QSqlQuery();

    if (!db.isOpen())
        return QSqlQuery();

    QString sql = QStringLiteral("SELECT date, characters_typed, error_count, elapsed_time, lesson_id FROM training_stats WHERE profile_id = ?");

    if (courseFilter)
    {
        sql += QLatin1String(" AND course_id = ?");
    }

    if (lessonFilter)
    {
        sql += QLatin1String(" AND lesson_id = ?");
    }

    QSqlQuery query(db);

    query.prepare(sql);

    query.bindValue(0, profile->id());

    if (courseFilter)
    {
        query.bindValue(1, courseFilter->id());
    }

    if (lessonFilter)
    {
        query.bindValue(courseFilter? 2: 1, lessonFilter->id());
    }

    if (!query.exec())
    {
        qWarning() <<  query.lastError().text();
        raiseError(query.lastError());
        return QSqlQuery();
    }

    return query;
}

int ProfileDataAccess::findCourseProgressId(Profile* profile, const QString& courseId, CourseProgressType type, bool* ok)
{
    *ok = false;

    QSqlDatabase db = database();

    if (!db.isOpen())
        return -1;

    QSqlQuery findQuery;

    findQuery.prepare(QStringLiteral("SELECT id FROM course_progress WHERE profile_id = ? AND course_id = ? AND type = ? LIMIT 1"));

    findQuery.bindValue(0, profile->id());
    findQuery.bindValue(1, courseId);
    findQuery.bindValue(2, type);

    if (!findQuery.exec())
    {
        qWarning() << findQuery.lastError().text();
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
