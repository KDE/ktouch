/*
 * Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
