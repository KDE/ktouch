/*
 * SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef USERDATAACCESS_H
#define USERDATAACCESS_H

#include "core/dbaccess.h"

class DataIndex;
class Course;
class KeyboardLayout;

class UserDataAccess : public DbAccess
{
    Q_OBJECT
public:
    explicit UserDataAccess(QObject* parent = 0);
    Q_INVOKABLE bool fillDataIndex(DataIndex* target);
    Q_INVOKABLE bool loadCourse(const QString& id, Course* target);
    Q_INVOKABLE bool storeCourse(Course* course);
    Q_INVOKABLE bool deleteCourse(Course* course);
    Q_INVOKABLE bool loadKeyboardLayout(const QString& id, KeyboardLayout* target);
    Q_INVOKABLE bool storeKeyboardLayout(KeyboardLayout* keyboardLayout);
    Q_INVOKABLE bool deleteKeyboardLayout(KeyboardLayout* keyboardLayout);
};

#endif // USERDATAACCESS_H
