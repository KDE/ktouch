#include "dataindex.h"

DataIndexCourse::DataIndexCourse(QObject* parent):
    CourseBase(parent)
{
}

DataIndexKeyboardLayout::DataIndexKeyboardLayout(QObject* parent):
    KeyboardLayoutBase(parent)
{
}

DataIndex::DataIndex(QObject* parent):
    QObject(parent)
{
}

unsigned int DataIndex::courseCount() const
{
    return m_courses.length();
}

DataIndexCourse* DataIndex::course(unsigned int index) const
{
    Q_ASSERT(index < m_courses.length());
    return m_courses.at(index);
}

void DataIndex::addCourse(DataIndexCourse *course)
{
    m_courses.append(course);
    course->setParent(this);
    emit courseCountChanged();
}

void DataIndex::removeCourse(unsigned int index)
{
    Q_ASSERT(index < m_courses.length());
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

unsigned int DataIndex::keyboardLayoutCount() const
{
    return m_keyboardLayouts.length();
}

DataIndexKeyboardLayout* DataIndex::keyboardLayout(unsigned int index) const
{
    Q_ASSERT(index < m_keyboardLayouts.length());
    return m_keyboardLayouts.at(index);
}

void DataIndex::addKeyboardLayout(DataIndexKeyboardLayout *keyboardLayout)
{
    m_keyboardLayouts.append(keyboardLayout);
    keyboardLayout->setParent(this);
    emit keyboardLayoutCountChanged();
}

void DataIndex::removeKeyboardLayout(unsigned int index)
{
    Q_ASSERT(index < m_keyboardLayouts.length());
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
