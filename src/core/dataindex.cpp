/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "dataindex.h"

DataIndex::DataIndex(QObject* parent):
    Resource(parent)
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

void DataIndex::addCourse(DataIndexCourse* course)
{
    Q_EMIT courseAboutToBeAdded(course, m_courses.length());
    m_courses.append(course);
    course->setParent(this);
    Q_EMIT courseCountChanged();
    Q_EMIT courseAdded();
}

void DataIndex::removeCourse(int index)
{
    Q_ASSERT(index >= 0 && index < m_courses.length());
    Q_EMIT coursesAboutToBeRemoved(index, index);
    delete m_courses.at(index);
    m_courses.removeAt(index);
    Q_EMIT courseCountChanged();
    Q_EMIT coursesRemoved();
}

void DataIndex::clearCourses()
{
    if (m_courses.isEmpty())
        return;

    Q_EMIT coursesAboutToBeRemoved(0, m_courses.length() - 1);
    qDeleteAll(m_courses);
    m_courses.clear();
    Q_EMIT courseCountChanged();
    Q_EMIT coursesRemoved();
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
    Q_EMIT keyboardLayoutAboutToBeAdded(keyboardLayout, m_keyboardLayouts.length());
    m_keyboardLayouts.append(keyboardLayout);
    keyboardLayout->setParent(this);
    Q_EMIT keyboardLayoutCountChanged();
    Q_EMIT keyboardLayoutAdded();
}

void DataIndex::removeKeyboardLayout(int index)
{
    Q_ASSERT(index >= 0 && index < m_keyboardLayouts.length());
    Q_EMIT keyboardLayoutsAboutToBeRemoved(index, index);
    delete m_keyboardLayouts.at(index);
    m_keyboardLayouts.removeAt(index);
    Q_EMIT keyboardLayoutCountChanged();
    Q_EMIT keyboardLayoutsRemoved();
}

void DataIndex::clearKeyboardLayouts()
{
    if (m_keyboardLayouts.isEmpty())
        return;

    Q_EMIT keyboardLayoutsAboutToBeRemoved(0, m_keyboardLayouts.length() - 1);
    qDeleteAll(m_keyboardLayouts);
    m_keyboardLayouts.clear();
    Q_EMIT keyboardLayoutCountChanged();
    Q_EMIT keyboardLayoutsRemoved();
}

DataIndexCourse::DataIndexCourse(QObject* parent):
    CourseBase(parent),
    m_source(DataIndex::BuiltInResource)
{
    setIsValid(true);
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
        Q_EMIT pathChanged();
    }
}

DataIndex::Source DataIndexCourse::source() const
{
    return m_source;
}

void DataIndexCourse::setSource(DataIndex::Source source)
{
    if (source != m_source)
    {
        m_source = source;
        Q_EMIT sourceChanged();
    }
}

DataIndexKeyboardLayout::DataIndexKeyboardLayout(QObject* parent):
    KeyboardLayoutBase(parent),
    m_source(DataIndex::BuiltInResource)
{
    setIsValid(true);
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
        Q_EMIT pathChanged();
    }
}

DataIndex::Source DataIndexKeyboardLayout::source() const
{
    return m_source;
}

void DataIndexKeyboardLayout::setSource(DataIndex::Source source)
{
    if (source != m_source)
    {
        m_source = source;
        Q_EMIT sourceChanged();
    }
}

#include "moc_dataindex.cpp"
