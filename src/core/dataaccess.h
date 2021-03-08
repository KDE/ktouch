/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef DATAACCESS_H
#define DATAACCESS_H

#include <QObject>

class Course;
class DataIndex;
class DataIndexCourse;
class DataIndexKeyboardLayout;
class KeyboardLayout;

class DataAccess : public QObject
{
    Q_OBJECT
public:
    explicit DataAccess(QObject* parent = 0);
    Q_INVOKABLE bool loadDataIndex(DataIndex* target);
    Q_INVOKABLE bool loadCourse(DataIndexCourse* dataIndexCourse, Course* target);
    Q_INVOKABLE bool loadKeyboardLayout(DataIndexKeyboardLayout* dataIndexKeyboardLayout, KeyboardLayout* target);
};

#endif // DATAACCESS_H
