/*
 * SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "dbaccess.h"

#include <QDebug>
#include <QDir>
#include <QUuid>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>

#include <KLocalizedString>


DbAccess::DbAccess(QObject* parent) :
    QObject(parent),
    m_errorMessage(QString())
{
}

QString DbAccess::errorMessage() const
{
    return m_errorMessage;
}

QSqlDatabase DbAccess::database()
{
    if (!QSqlDatabase::contains(QSqlDatabase::defaultConnection))
    {
        QDir dataDir = QDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
        if (!dataDir.exists())
        {
            dataDir.mkpath(dataDir.path());
        }
        QString dbPath = dataDir.filePath(QStringLiteral("profiles.db"));
        QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));
        db.setDatabaseName(dbPath);
        if (!db.open())
        {
            qWarning() << db.lastError().text();
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

void DbAccess::raiseError(const QSqlError& error)
{
    m_errorMessage = QStringLiteral("%1: %2").arg(error.driverText(), error.databaseText());
    emit errorMessageChanged();
}

bool DbAccess::checkDbSchema()
{
    QSqlDatabase db = QSqlDatabase::database();

    db.exec("CREATE TABLE IF NOT EXISTS metadata ("
            "key TEXT PRIMARY KEY, "
            "value TEXT"
            ")");

    if (db.lastError().isValid())
    {
        qWarning() << db.lastError().text();
        raiseError(db.lastError());
        return false;
    }

    QSqlQuery versionQuery = db.exec(QStringLiteral("SELECT value FROM metadata WHERE key = 'version'"));

    if (db.lastError().isValid())
    {
        qWarning() << db.lastError().text();
        raiseError(db.lastError());
        return false;
    }

    if (versionQuery.next())
    {
        QString version = versionQuery.value(0).toString();

        versionQuery.clear();

        if (version == QLatin1String("1.0"))
        {
            return migrateFrom1_0To1_1();
        }

        if (version != QLatin1String("1.1"))
        {
            m_errorMessage = i18n("Invalid database version '%1'.", version);
            emit errorMessageChanged();
            return false;
        }
    }
    else
    {
        if (!db.transaction())
        {
            qWarning() <<  db.lastError().text();
            raiseError(db.lastError());
            return false;
        }
        db.exec(QStringLiteral("INSERT INTO metadata (key, value) VALUES ('version', '1.1')"));
        if (db.lastError().isValid())
        {
            qWarning() << db.lastError().text();
            raiseError(db.lastError());
            return false;
        }
        if (!db.commit())
        {
            qWarning() << db.lastError().text();
            raiseError(db.lastError());
            return false;
        }
    }

    db.exec("CREATE TABLE IF NOT EXISTS profiles ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT, "
            "skill_level INTEGER, "
            "last_used_course_id TEXT "
            ")");

    if (db.lastError().isValid())
    {
        qWarning() << db.lastError().text();
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
            "elapsed_time INTEGER "
            ")");

    if (db.lastError().isValid())
    {
        qWarning() << db.lastError().text();
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
        qWarning() << db.lastError().text();
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
        qWarning() << db.lastError().text();
        raiseError(db.lastError());
        return false;
    }

    db.exec("CREATE TABLE IF NOT EXISTS courses ("
            "id TEXT PRIMARY KEY, "
            "title TEXT, "
            "description TEXT, "
            "keyboard_layout_name TEXT "
            ")");

    if (db.lastError().isValid())
    {
        qWarning() << db.lastError().text();
        raiseError(db.lastError());
        return false;
    }

    db.exec("CREATE TABLE IF NOT EXISTS course_lessons ("
            "id TEXT PRIMARY KEY, "
            "course_id TEXT, "
            "title TEXT, "
            "new_characters TEXT, "
            "text TEXT "
            ")");

    if (db.lastError().isValid())
    {
        qWarning() << db.lastError().text();
        raiseError(db.lastError());
        return false;
    }

    db.exec("CREATE TABLE IF NOT EXISTS keyboard_layouts ("
            "id TEXT PRIMARY KEY, "
            "title TEXT, "
            "name TEXT, "
            "width INTEGER, "
            "height INTEGER "
            ")");

    if (db.lastError().isValid())
    {
        qWarning() << db.lastError().text();
        raiseError(db.lastError());
        return false;
    }

    db.exec("CREATE TABLE IF NOT EXISTS keyboard_layout_keys ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "keyboard_layout_id TEXT, "
            "left INTEGER, "
            "top INTEGER, "
            "width INTEGER, "
            "height INTEGER, "
            "type INTEGER, "
            "finger_index INTEGER, "
            "has_haptic_marker INTEGER, "
            "special_key_type TEXT, "
            "modifier_id TEXT, "
            "label TEXT"
            ")");

    if (db.lastError().isValid())
    {
        qWarning() << db.lastError().text();
        raiseError(db.lastError());
        return false;
    }

    db.exec("CREATE TABLE IF NOT EXISTS keyboard_layout_key_chars ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "key_id INTEGER, "
            "position INTEGER, "
            "character TEXT, "
            "modifier TEXT "
            ")");

    if (db.lastError().isValid())
    {
        qWarning() << db.lastError().text();
        raiseError(db.lastError());
        return false;
    }

    db.exec("CREATE TABLE IF NOT EXISTS custom_lessons ("
            "id TEXT PRIMARY KEY, "
            "profile_id INTEGER, "
            "title TEXT, "
            "text TEXT, "
            "keyboard_layout_name TEXT "
            ")");

    if (db.lastError().isValid())
    {
        qWarning() << db.lastError().text();
        raiseError(db.lastError());
        return false;
    }

    return true;
}

bool DbAccess::migrateFrom1_0To1_1()
{
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.transaction())
    {
        qWarning() <<  db.lastError().text();
        raiseError(db.lastError());
        db.rollback();
        return false;
    }

    db.exec("ALTER TABLE course_lessons "
            "RENAME TO course_lessons_backup");

    if (db.lastError().isValid())
    {
        qWarning() << db.lastError().text();
        raiseError(db.lastError());
        db.rollback();
        return false;
    }

    db.exec("CREATE TABLE course_lessons ("
            "id TEXT PRIMARY KEY, "
            "course_id TEXT, "
            "title TEXT, "
            "new_characters TEXT, "
            "text TEXT "
            ")");

    if (db.lastError().isValid())
    {
        qWarning() << db.lastError().text();
        raiseError(db.lastError());
        db.rollback();
        return false;
    }

    db.exec("INSERT INTO course_lessons (id, course_id, title, new_characters, text) "
            "SELECT id, course_id, title, new_characters, text FROM course_lessons_backup ");

    if (db.lastError().isValid())
    {
        qWarning() << db.lastError().text();
        raiseError(db.lastError());
        db.rollback();
        return false;
    }

    db.exec(QStringLiteral("DROP TABLE course_lessons_backup"));

    if (db.lastError().isValid())
    {
        qWarning() << db.lastError().text();
        raiseError(db.lastError());
        db.rollback();
        return false;
    }

    // generate new UUIDs for lessons

    QSqlQuery idsQuery = db.exec(QStringLiteral("SELECT id FROM course_lessons"));

    if (db.lastError().isValid())
    {
        qWarning() << db.lastError().text();
        raiseError(db.lastError());
        db.rollback();
        return false;
    }

    QStringList ids;

    while (idsQuery.next())
    {
        ids << idsQuery.value(0).toString();
    }

    QSqlQuery updateIdQuery(db);

    updateIdQuery.prepare(QStringLiteral("UPDATE course_lessons SET id = ? WHERE id = ?"));

    foreach (const QString& id, ids)
    {
        updateIdQuery.bindValue(0, QUuid::createUuid().toString());
        updateIdQuery.bindValue(1, id);

        updateIdQuery.exec();

        if (updateIdQuery.lastError().isValid())
        {
            qWarning() << updateIdQuery.lastError().text();
            raiseError(updateIdQuery.lastError());
            db.rollback();
            return false;
        }
    }

    db.exec(QStringLiteral("UPDATE metadata SET value = '1.1' WHERE key = 'version'"));

    if (db.lastError().isValid())
    {
        qWarning() << db.lastError().text();
        raiseError(db.lastError());
        db.rollback();
        return false;
    }

    if (!db.commit())
    {
        qWarning() << db.lastError().text();
        raiseError(db.lastError());
        db.rollback();
        return false;
    }

    return true;
}
