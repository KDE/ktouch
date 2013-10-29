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

#include "course.h"

#include <QSignalMapper>

#include <kstandarddirs.h>
#include <klocale.h>
#include <kdebug.h>

#include "lesson.h"
#include "dataindex.h"

Course::Course(QObject *parent) :
    CourseBase(parent),
    m_associatedDataIndexCourse(0),
    m_doSyncLessonCharacters(true),
    m_signalMapper(new QSignalMapper(this))
{
    connect(m_signalMapper, SIGNAL(mapped(int)), SLOT(updateLessonCharacters(int)));
}

DataIndexCourse* Course::associatedDataIndexCourse() const
{
    return m_associatedDataIndexCourse;
}

void Course::setAssociatedDataIndexCourse(DataIndexCourse* dataIndexCourse)
{
    if (dataIndexCourse != m_associatedDataIndexCourse)
    {
        m_associatedDataIndexCourse = dataIndexCourse;
        emit associatedDataIndexCourseChanged();
    }
}

bool Course::doSyncLessonCharacters() const
{
    return m_doSyncLessonCharacters;
}

void Course::setDoSyncLessonCharacters(bool doSync)
{
    if (doSync != m_doSyncLessonCharacters)
    {
        m_doSyncLessonCharacters = doSync;
        emit doSyncLessonCharactersChanged();
    }
}

void Course::setId(const QString& id)
{
    CourseBase::setId(id);

    if (m_associatedDataIndexCourse)
    {
        m_associatedDataIndexCourse->setId(id);
    }
}

void Course::setTitle(const QString &title)
{
    CourseBase::setTitle(title);

    if (m_associatedDataIndexCourse)
    {
        m_associatedDataIndexCourse->setTitle(title);
    }
}

void Course::setDescription(const QString &description)
{
    CourseBase::setDescription(description);

    if (m_associatedDataIndexCourse)
    {
        m_associatedDataIndexCourse->setDescription(description);
    }
}

void Course::setKeyboardLayoutName(const QString &keyboardLayoutName)
{
    CourseBase::setKeyboardLayoutName(keyboardLayoutName);

    if (m_associatedDataIndexCourse)
    {
        m_associatedDataIndexCourse->setKeyboardLayoutName(keyboardLayoutName);
    }
}

int Course::lessonCount() const
{
    return m_lessons.count();
}

Lesson* Course::lesson(int index) const
{
    Q_ASSERT(index >= 0 && index < m_lessons.count());
    return m_lessons.at(index);
}

void Course::addLesson(Lesson* lesson)
{
    emit lessonAboutToBeAdded(lesson, m_lessons.length());
    m_lessons.append(lesson);
    lesson->setParent(this);
    const int index = m_lessons.length() - 1;
    updateLessonCharacters(index);
    connect(lesson, SIGNAL(newCharactersChanged()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(lesson, index);
    emit lessonCountChanged();
    emit lessonAdded();
}

void Course::insertLesson(int index, Lesson* lesson)
{
    Q_ASSERT(index >= 0 && index < m_lessons.count());
    emit lessonAboutToBeAdded(lesson, index);
    m_lessons.insert(index, lesson);
    lesson->setParent(this);
    updateLessonCharacters(index);
    connect(lesson, SIGNAL(newCharactersChanged()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(lesson, index);
    emit lessonCountChanged();
    emit lessonAdded();
}

void Course::removeLesson(int index)
{
    Q_ASSERT(index >= 0 && index < m_lessons.count());
    emit lessonsAboutToBeRemoved(index, index);
    Lesson* const lesson = m_lessons.at(index);
    m_lessons.removeAt(index);
    delete lesson;
    updateLessonCharacters(index);
    emit lessonCountChanged();
    emit lessonsRemoved();
}

void Course::clearLessons()
{
    if (m_lessons.count() == 0)
        return;

    emit lessonsAboutToBeRemoved(0, m_lessons.length() - 1);
    qDeleteAll(m_lessons);
    m_lessons.clear();
    emit lessonCountChanged();
    emit lessonsRemoved();
}

void Course::copyFrom(Course* source)
{
    setIsValid(false);
    setId(source->id());
    setTitle(source->title());
    setDescription(source->description());
    setKeyboardLayoutName(source->keyboardLayoutName());
    setDoSyncLessonCharacters(source->doSyncLessonCharacters());
    clearLessons();
    for (int i = 0; i < source->lessonCount(); i++)
    {
        Lesson* lesson = new Lesson(this);
        lesson->copyFrom(source->lesson(i));
        addLesson(lesson);
    }
    setIsValid(true);
}

void Course::updateLessonCharacters(int firstIndex)
{
    if (!m_doSyncLessonCharacters)
        return;

    QString characters = firstIndex > 0? lesson(firstIndex - 1)->characters(): "";

    for (int i = firstIndex; i < lessonCount(); i++)
    {
        Lesson* const lesson = this->lesson(i);
        const QString newChars = lesson->newCharacters();
        for (int j = 0; j < newChars.length(); j++)
        {
            const QChar newChar = newChars.at(j);
            if (!characters.contains(newChar))
                characters.append(newChar);
        }
        lesson->setCharacters(characters);
    }
}
