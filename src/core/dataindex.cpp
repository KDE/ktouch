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

#include "dataindex.h"

DataIndexCourse::DataIndexCourse(QObject* parent):
    CourseBase(parent)
{
}

QString DataIndexCourse::path() const
{
    return m_path;
}

void DataIndexCourse::setPath(const QString& path)
{
    if(path != m_path)
    {
        m_path = path;
        emit pathChanged();
    }
}

DataIndexKeyboardLayout::DataIndexKeyboardLayout(QObject* parent):
    KeyboardLayoutBase(parent)
{
}

QString DataIndexKeyboardLayout::path() const
{
    return m_path;
}

void DataIndexKeyboardLayout::setPath(const QString& path)
{
    if(path != m_path)
    {
        m_path = path;
        emit pathChanged();
    }
}

DataIndex::DataIndex(QObject* parent):
    QObject(parent)
{
}

int DataIndex::courseCount() const
{
    return m_courses.length();
}

DataIndexCourse* DataIndex::course(int index) const
{
    Q_ASSERT(index >= 0 && index < m_courses.length());
    return m_courses.at(index);
}

void DataIndex::addCourse(DataIndexCourse *course)
{
    m_courses.append(course);
    course->setParent(this);
    emit courseCountChanged();
}

void DataIndex::removeCourse(int index)
{
    Q_ASSERT(index >= index < m_courses.length());
    delete m_courses.at(index);
    m_courses.removeAt(index);
    emit courseCountChanged();
}

void DataIndex::clearCourses()
{
    qDeleteAll(m_courses);
    m_courses.clear();
    emit courseCountChanged();
}

int DataIndex::keyboardLayoutCount() const
{
    return m_keyboardLayouts.length();
}

DataIndexKeyboardLayout* DataIndex::keyboardLayout(int index) const
{
    Q_ASSERT(index >= 0 && index < m_keyboardLayouts.length());
    return m_keyboardLayouts.at(index);
}

void DataIndex::addKeyboardLayout(DataIndexKeyboardLayout *keyboardLayout)
{
    m_keyboardLayouts.append(keyboardLayout);
    keyboardLayout->setParent(this);
    emit keyboardLayoutCountChanged();
}

void DataIndex::removeKeyboardLayout(int index)
{
    Q_ASSERT(index >= 0 && index < m_keyboardLayouts.length());
    delete m_keyboardLayouts.at(index);
    m_keyboardLayouts.removeAt(index);
    emit keyboardLayoutCountChanged();
}

void DataIndex::clearKeyboardLayouts()
{
    qDeleteAll(m_keyboardLayouts);
    m_keyboardLayouts.clear();
    emit keyboardLayoutCountChanged();
}
