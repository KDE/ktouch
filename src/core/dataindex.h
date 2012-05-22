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

#ifndef DATAINDEX_H
#define DATAINDEX_H

#include <QObject>

#include <QString>
#include <QList>

#include "coursebase.h"
#include "keyboardlayoutbase.h"

class DataIndexCourse: public CourseBase
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)

public:
    explicit DataIndexCourse(QObject* parent = 0);
    QString path() const;
    void setPath(const QString& path);

signals:
    void pathChanged();

private:
    QString m_path;
};

class DataIndexKeyboardLayout: public KeyboardLayoutBase
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)

public:
    explicit DataIndexKeyboardLayout(QObject* parent = 0);

    QString path() const;
    void setPath(const QString& path);

signals:
    void pathChanged();

private:
    QString m_path;
};

class DataIndex : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int courseCount READ courseCount NOTIFY courseCountChanged)
    Q_PROPERTY(int keyboardLayoutCount READ keyboardLayoutCount NOTIFY keyboardLayoutCountChanged)

public:
    explicit DataIndex(QObject* parent = 0);
    int courseCount() const;
    Q_INVOKABLE DataIndexCourse* course(int index) const;
    Q_INVOKABLE void addCourse(DataIndexCourse* course);
    Q_INVOKABLE void removeCourse(int index);
    Q_INVOKABLE void clearCourses();
    int keyboardLayoutCount() const;
    Q_INVOKABLE DataIndexKeyboardLayout* keyboardLayout(int index) const;
    Q_INVOKABLE void addKeyboardLayout(DataIndexKeyboardLayout* keyboardLayout);
    Q_INVOKABLE void removeKeyboardLayout(int index);
    Q_INVOKABLE void clearKeyboardLayouts();

signals:
    void courseCountChanged();
    void keyboardLayoutCountChanged();

private:
    QList<DataIndexCourse*> m_courses;
    QList<DataIndexKeyboardLayout*> m_keyboardLayouts;
};

#endif // DATAINDEX_H
