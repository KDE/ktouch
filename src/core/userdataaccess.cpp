/*
 * SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "userdataaccess.h"

#include <QDebug>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <KLocalizedString>

#include "core/dataindex.h"
#include "core/course.h"
#include "core/lesson.h"
#include "core/keyboardlayout.h"
#include "core/key.h"
#include "core/keychar.h"
#include "core/specialkey.h"

enum KeyTypeId
{
    KeyId = 1,
    SpecialKeyId
};

UserDataAccess::UserDataAccess(QObject* parent) :
    DbAccess(parent)
{
}

bool UserDataAccess::fillDataIndex(DataIndex* target)
{
    QSqlDatabase db = database();

    if (!db.isOpen())
        return false;

    QSqlQuery courseQuery = db.exec(QStringLiteral("SELECT id, title, description, keyboard_layout_name FROM courses"));

    if (courseQuery.lastError().isValid())
    {
        qWarning() << courseQuery.lastError().text();
        raiseError(courseQuery.lastError());
        return false;
    }

    while (courseQuery.next())
    {
        DataIndexCourse* course = new DataIndexCourse();

        course->setId(courseQuery.value(0).toString());
        course->setTitle(courseQuery.value(1).toString());
        course->setDescription(courseQuery.value(2).toString());
        course->setKeyboardLayoutName(courseQuery.value(3).toString());
        course->setSource(DataIndex::UserResource);

        target->addCourse(course);
    }

    QSqlQuery keyboardLayoutQuery = db.exec(QStringLiteral("SELECT id, title, name FROM keyboard_layouts"));

    if (keyboardLayoutQuery.lastError().isValid())
    {
        qWarning() << keyboardLayoutQuery.lastError().text();
        raiseError(keyboardLayoutQuery.lastError());
        return false;
    }

    while (keyboardLayoutQuery.next())
    {
        DataIndexKeyboardLayout* keyboardLayout = new DataIndexKeyboardLayout();

        keyboardLayout->setId(keyboardLayoutQuery.value(0).toString());
        keyboardLayout->setTitle(keyboardLayoutQuery.value(1).toString());
        keyboardLayout->setName(keyboardLayoutQuery.value(2).toString());
        keyboardLayout->setSource(DataIndex::UserResource);

        target->addKeyboardLayout(keyboardLayout);
    }

    return true;
}

bool UserDataAccess::loadCourse(const QString& id, Course* target)
{
    target->setIsValid(false);

    QSqlDatabase db = database();

    if (!db.isOpen())
        return false;

    QSqlQuery courseQuery(db);

    courseQuery.prepare(QStringLiteral("SELECT title, description, keyboard_layout_name FROM courses WHERE id = ? LIMIT 1"));
    courseQuery.bindValue(0, id);
    courseQuery.exec();

    if (courseQuery.lastError().isValid())
    {
        qWarning() << courseQuery.lastError().text();
        raiseError(courseQuery.lastError());
        return false;
    }

    if (!courseQuery.next())
    {
        const QString warning = i18n("No course with ID %1", id);
        qWarning() << warning;
        raiseError(warning);
    }

    target->setId(id);
    target->setTitle(courseQuery.value(0).toString());
    target->setDescription(courseQuery.value(1).toString());
    target->setKeyboardLayoutName(courseQuery.value(2).toString());
    target->setKind(Course::SequentialCourse);
    target->clearLessons();

    QSqlQuery lessonsQuery(db);

    lessonsQuery.prepare(QStringLiteral("SELECT id, title, new_characters, text FROM course_lessons WHERE course_id = ?"));
    lessonsQuery.bindValue(0, id);
    lessonsQuery.exec();

    if (lessonsQuery.lastError().isValid())
    {
        qWarning() << lessonsQuery.lastError().text();
        raiseError(lessonsQuery.lastError());
        return false;
    }

    while (lessonsQuery.next())
    {
        Lesson* lesson = new Lesson();

        lesson->setId(lessonsQuery.value(0).toString());
        lesson->setTitle(lessonsQuery.value(1).toString());
        lesson->setNewCharacters(lessonsQuery.value(2).toString());
        lesson->setText(lessonsQuery.value(3).toString());

        target->addLesson(lesson);
    }

    target->setIsValid(true);

    return true;
}

bool UserDataAccess::storeCourse(Course* course)
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

    QSqlQuery cleanUpCourseQuery(db);

    cleanUpCourseQuery.prepare(QStringLiteral("DELETE FROM courses WHERE id = ?"));
    cleanUpCourseQuery.bindValue(0, course->id());
    cleanUpCourseQuery.exec();

    if (cleanUpCourseQuery.lastError().isValid())
    {
        qWarning() << cleanUpCourseQuery.lastError().text();
        raiseError(cleanUpCourseQuery.lastError());
        db.rollback();
        return false;
    }

    QSqlQuery cleanUpLessonsQuery(db);

    cleanUpLessonsQuery.prepare(QStringLiteral("DELETE FROM course_lessons WHERE course_id = ?"));
    cleanUpLessonsQuery.bindValue(0, course->id());
    cleanUpLessonsQuery.exec();

    if (cleanUpLessonsQuery.lastError().isValid())
    {
        qWarning() << cleanUpLessonsQuery.lastError().text();
        raiseError(cleanUpLessonsQuery.lastError());
        db.rollback();
        return false;
    }

    QSqlQuery insertCourseQuery(db);

    insertCourseQuery.prepare(QStringLiteral("INSERT INTO courses (id, title, description, keyboard_layout_name) VALUES (?, ?, ?, ?)"));
    insertCourseQuery.bindValue(0, course->id());
    insertCourseQuery.bindValue(1, course->title());
    insertCourseQuery.bindValue(2, course->description());
    insertCourseQuery.bindValue(3, course->keyboardLayoutName());
    insertCourseQuery.exec();

    if (insertCourseQuery.lastError().isValid())
    {
        qWarning() << insertCourseQuery.lastError().text();
        raiseError(insertCourseQuery.lastError());
        db.rollback();
        return false;
    }

    QSqlQuery insertLessonsQuery(db);

    insertLessonsQuery.prepare(QStringLiteral("INSERT INTO course_lessons (id, title, new_characters, text, course_id) VALUES(?, ?, ?, ?, ?)"));

    insertLessonsQuery.bindValue(4, course->id());

    for (int i = 0; i < course->lessonCount(); i++)
    {
        Lesson* lesson = course->lesson(i);

        insertLessonsQuery.bindValue(0, lesson->id());
        insertLessonsQuery.bindValue(1, lesson->title());
        insertLessonsQuery.bindValue(2, lesson->newCharacters());
        insertLessonsQuery.bindValue(3, lesson->text());
        insertLessonsQuery.exec();

        if (insertLessonsQuery.lastError().isValid())
        {
            qWarning() << insertLessonsQuery.lastError().text();
            raiseError(insertLessonsQuery.lastError());
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

bool UserDataAccess::deleteCourse(Course* course)
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

    QSqlQuery deleteCourseQuery(db);

    deleteCourseQuery.prepare(QStringLiteral("DELETE FROM courses WHERE id = ?"));
    deleteCourseQuery.bindValue(0, course->id());
    deleteCourseQuery.exec();

    if (deleteCourseQuery.lastError().isValid())
    {
        qWarning() << deleteCourseQuery.lastError().text();
        raiseError(deleteCourseQuery.lastError());
        db.rollback();
        return false;
    }

    QSqlQuery deleteLessonsQuery(db);

    deleteLessonsQuery.prepare(QStringLiteral("DELETE FROM course_lessons WHERE course_id = ?"));
    deleteLessonsQuery.bindValue(0, course->id());
    deleteLessonsQuery.exec();

    if (deleteLessonsQuery.lastError().isValid())
    {
        qWarning() << deleteLessonsQuery.lastError().text();
        raiseError(deleteLessonsQuery.lastError());
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

bool UserDataAccess::loadKeyboardLayout(const QString& id, KeyboardLayout* target)
{
    target->setIsValid(false);

    QSqlDatabase db = database();

    if (!db.isOpen())
        return false;

    QSqlQuery keyboardLayoutQuery(db);

    keyboardLayoutQuery.prepare(QStringLiteral("SELECT title, name, width, height FROM keyboard_layouts WHERE id = ? LIMIT 1"));
    keyboardLayoutQuery.bindValue(0, id);
    keyboardLayoutQuery.exec();


    if (keyboardLayoutQuery.lastError().isValid())
    {
        qWarning() << keyboardLayoutQuery.lastError().text();
        raiseError(keyboardLayoutQuery.lastError());
        return false;
    }

    if (!keyboardLayoutQuery.next())
    {
        const QString warning = i18n("No keyboard layout with ID %1", id);
        qWarning() << warning;
        raiseError(warning);
    }

    target->setId(id);
    target->setTitle(keyboardLayoutQuery.value(0).toString());
    target->setName(keyboardLayoutQuery.value(1).toString());
    target->setWidth(keyboardLayoutQuery.value(2).toInt());
    target->setHeight(keyboardLayoutQuery.value(3).toInt());
    target->clearKeys();

    QSqlQuery keysQuery(db);

    keysQuery.prepare(QStringLiteral("SELECT id, left, top, width, height, type, finger_index, has_haptic_marker, special_key_type, modifier_id, label FROM keyboard_layout_keys WHERE keyboard_layout_id = ?"));
    keysQuery.bindValue(0, id);
    keysQuery.exec();

    QSqlQuery keyCharsQuery(db);

    keyCharsQuery.prepare(QStringLiteral("SELECT position, character, modifier FROM keyboard_layout_key_chars WHERE key_id = ?"));

    if (keysQuery.lastError().isValid())
    {
        qWarning() << keysQuery.lastError().text();
        raiseError(keysQuery.lastError());
        return false;
    }

    while (keysQuery.next())
    {
        AbstractKey* abstractKey;

        KeyTypeId keyType =  static_cast<KeyTypeId>(keysQuery.value(5).toInt());

        if (keyType == KeyId)
        {
            Key* key = new Key();

            key->setFingerIndex(keysQuery.value(6).toInt());
            key->setHasHapticMarker(keysQuery.value(7).toBool());

            keyCharsQuery.bindValue(0, keysQuery.value(0));
            keyCharsQuery.exec();

            if (keyCharsQuery.lastError().isValid())
            {
                qWarning() << keyCharsQuery.lastError().text();
                raiseError(keyCharsQuery.lastError());
                return false;
            }

            while (keyCharsQuery.next())
            {
                KeyChar* keyChar = new KeyChar();

                keyChar->setPosition(static_cast<KeyChar::Position>(keyCharsQuery.value(0).toInt()));
                keyChar->setValue(keyCharsQuery.value(1).toString().at(0));
                keyChar->setModifier(keyCharsQuery.value(2).toString());

                key->addKeyChar(keyChar);
            }

            abstractKey = key;
        }
        else
        {
            SpecialKey* specialKey = new SpecialKey();

            specialKey->setTypeStr(keysQuery.value(8).toString());
            specialKey->setModifierId(keysQuery.value(9).toString());
            specialKey->setLabel(keysQuery.value(10).toString());

            abstractKey = specialKey;
        }

        abstractKey->setLeft(keysQuery.value(1).toInt());
        abstractKey->setTop(keysQuery.value(2).toInt());
        abstractKey->setWidth(keysQuery.value(3).toInt());
        abstractKey->setHeight(keysQuery.value(4).toInt());

        target->addKey(abstractKey);
    }

    target->setIsValid(true);

    return true;
}

bool UserDataAccess::storeKeyboardLayout(KeyboardLayout* keyboardLayout)
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

    QSqlQuery cleanUpKeyCharsQuery(db);

    cleanUpKeyCharsQuery.prepare(QStringLiteral("DELETE FROM keyboard_layout_key_chars WHERE key_id IN (SELECT id FROM keyboard_layout_keys WHERE keyboard_layout_id = ?)"));
    cleanUpKeyCharsQuery.bindValue(0, keyboardLayout->id());
    cleanUpKeyCharsQuery.exec();

    if (cleanUpKeyCharsQuery.lastError().isValid())
    {
        qWarning() << cleanUpKeyCharsQuery.lastError().text();
        raiseError(cleanUpKeyCharsQuery.lastError());
        db.rollback();
        return false;
    }


    QSqlQuery cleanUpKeysQuery(db);

    cleanUpKeysQuery.prepare(QStringLiteral("DELETE FROM keyboard_layout_keys WHERE keyboard_layout_id = ?"));
    cleanUpKeysQuery.bindValue(0, keyboardLayout->id());
    cleanUpKeysQuery.exec();

    if (cleanUpKeysQuery.lastError().isValid())
    {
        qWarning() << cleanUpKeysQuery.lastError().text();
        raiseError(cleanUpKeysQuery.lastError());
        db.rollback();
        return false;
    }

    QSqlQuery cleanUpKeyboardLayoutQuery(db);

    cleanUpKeyboardLayoutQuery.prepare(QStringLiteral("DELETE FROM keyboard_layouts WHERE id = ?"));
    cleanUpKeyboardLayoutQuery.bindValue(0, keyboardLayout->id());
    cleanUpKeyboardLayoutQuery.exec();

    if (cleanUpKeyboardLayoutQuery.lastError().isValid())
    {
        qWarning() << cleanUpKeyboardLayoutQuery.lastError().text();
        raiseError(cleanUpKeyboardLayoutQuery.lastError());
        db.rollback();
        return false;
    }

    QSqlQuery insertKeyboardLayoutQuery(db);

    insertKeyboardLayoutQuery.prepare(QStringLiteral("INSERT INTO keyboard_layouts (id, title, name, width, height) VALUES (?, ?, ?, ?, ?)"));
    insertKeyboardLayoutQuery.bindValue(0, keyboardLayout->id());
    insertKeyboardLayoutQuery.bindValue(1, keyboardLayout->title());
    insertKeyboardLayoutQuery.bindValue(2, keyboardLayout->name());
    insertKeyboardLayoutQuery.bindValue(3, keyboardLayout->width());
    insertKeyboardLayoutQuery.bindValue(4, keyboardLayout->height());
    insertKeyboardLayoutQuery.exec();

    if (insertKeyboardLayoutQuery.lastError().isValid())
    {
        qWarning() << insertKeyboardLayoutQuery.lastError().text();
        raiseError(insertKeyboardLayoutQuery.lastError());
        db.rollback();
        return false;
    }

    QSqlQuery insertKeyQuery(db);
    QSqlQuery insertSpecialKeyQuery(db);
    QSqlQuery insertKeyCharQuery(db);
    QSqlQuery idQuery(db);

    insertKeyQuery.prepare(QStringLiteral("INSERT INTO keyboard_layout_keys (keyboard_layout_id, left, top, width, height, type, finger_index, has_haptic_marker) VALUES (?, ?, ?, ?, ?, ?, ?, ?)"));
    insertKeyQuery.bindValue(0, keyboardLayout->id());
    insertKeyQuery.bindValue(5, KeyId);
    insertSpecialKeyQuery.prepare(QStringLiteral("INSERT INTO keyboard_layout_keys (keyboard_layout_id, left, top, width, height, type, special_key_type, modifier_id, label) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)"));
    insertSpecialKeyQuery.bindValue(0, keyboardLayout->id());
    insertSpecialKeyQuery.bindValue(5, SpecialKeyId);
    insertKeyCharQuery.prepare(QStringLiteral("INSERT INTO keyboard_layout_key_chars (key_id, position, character, modifier) VALUES (?, ?, ?, ?)"));
    idQuery.prepare(QStringLiteral("SELECT last_insert_rowid()"));

    for (int i = 0; i < keyboardLayout->keyCount(); i++)
    {
        AbstractKey* const abstractKey = keyboardLayout->key(i);

        if (Key* const key = qobject_cast<Key*>(abstractKey))
        {
            insertKeyQuery.bindValue(1, key->left());
            insertKeyQuery.bindValue(2, key->top());
            insertKeyQuery.bindValue(3, key->width());
            insertKeyQuery.bindValue(4, key->height());
            insertKeyQuery.bindValue(6, key->fingerIndex());
            insertKeyQuery.bindValue(7, key->hasHapticMarker());
            insertKeyQuery.exec();

            if (insertKeyQuery.lastError().isValid())
            {
                qWarning() << insertKeyQuery.lastError().text();
                raiseError(insertKeyQuery.lastError());
                db.rollback();
                return false;
            }

            idQuery.exec();

            if (idQuery.lastError().isValid())
            {
                qWarning() << idQuery.lastError().text();
                raiseError(idQuery.lastError());
                db.rollback();
                return false;
            }

            idQuery.next();

            const int keyId = idQuery.value(0).toInt();

            insertKeyCharQuery.bindValue(0, keyId);

            for (int j = 0; j < key->keyCharCount(); j++)
            {
                KeyChar * const keyChar = key->keyChar(j);

                insertKeyCharQuery.bindValue(1, keyChar->position());
                insertKeyCharQuery.bindValue(2, QString(keyChar->value()));
                insertKeyCharQuery.bindValue(3, keyChar->modifier());
                insertKeyCharQuery.exec();

                if (insertKeyCharQuery.lastError().isValid())
                {
                    qWarning() << insertKeyCharQuery.lastError().text();
                    raiseError(insertKeyCharQuery.lastError());
                    db.rollback();
                    return false;
                }
            }
        }

        if (SpecialKey* const specialKey = qobject_cast<SpecialKey*>(abstractKey))
        {
            insertSpecialKeyQuery.bindValue(1, specialKey->left());
            insertSpecialKeyQuery.bindValue(2, specialKey->top());
            insertSpecialKeyQuery.bindValue(3, specialKey->width());
            insertSpecialKeyQuery.bindValue(4, specialKey->height());
            insertSpecialKeyQuery.bindValue(6, specialKey->typeStr());
            insertSpecialKeyQuery.bindValue(7, specialKey->modifierId());
            insertSpecialKeyQuery.bindValue(8, specialKey->label());
            insertSpecialKeyQuery.exec();

            if (insertSpecialKeyQuery.lastError().isValid())
            {
                qWarning() << insertSpecialKeyQuery.lastError().text();
                raiseError(insertSpecialKeyQuery.lastError());
                db.rollback();
                return false;
            }
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

bool UserDataAccess::deleteKeyboardLayout(KeyboardLayout* keyboardLayout)
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

    QSqlQuery deleteKeyCharsQuery(db);

    deleteKeyCharsQuery.prepare(QStringLiteral("DELETE FROM keyboard_layout_key_chars WHERE key_id IN (SELECT id FROM keyboard_layout_keys WHERE keyboard_layout_id = ?)"));
    deleteKeyCharsQuery.bindValue(0, keyboardLayout->id());
    deleteKeyCharsQuery.exec();

    if (deleteKeyCharsQuery.lastError().isValid())
    {
        qWarning() << deleteKeyCharsQuery.lastError().text();
        raiseError(deleteKeyCharsQuery.lastError());
        db.rollback();
        return false;
    }


    QSqlQuery deleteKeysQuery(db);

    deleteKeysQuery.prepare(QStringLiteral("DELETE FROM keyboard_layout_keys WHERE keyboard_layout_id = ?"));
    deleteKeysQuery.bindValue(0, keyboardLayout->id());
    deleteKeysQuery.exec();

    if (deleteKeysQuery.lastError().isValid())
    {
        qWarning() << deleteKeysQuery.lastError().text();
        raiseError(deleteKeysQuery.lastError());
        db.rollback();
        return false;
    }

    QSqlQuery deleteKeyboardLayoutQuery(db);

    deleteKeyboardLayoutQuery.prepare(QStringLiteral("DELETE FROM keyboard_layouts WHERE id = ?"));
    deleteKeyboardLayoutQuery.bindValue(0, keyboardLayout->id());
    deleteKeyboardLayoutQuery.exec();

    if (deleteKeyboardLayoutQuery.lastError().isValid())
    {
        qWarning() << deleteKeyboardLayoutQuery.lastError().text();
        raiseError(deleteKeyboardLayoutQuery.lastError());
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
