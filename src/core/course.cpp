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

Course::Course(QObject *parent) :
    CourseBase(parent),
    m_signalMapper(new QSignalMapper(this))
{
    connect(m_signalMapper, SIGNAL(mapped(int)), SLOT(updateLessonCharacters(int)));
}

QString Course::id() const
{
    return m_id;
}

void Course::setId(const QString& id)
{
    if (id != m_id)
    {
        m_id = id;
        emit idChanged();
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
    m_lessons.append(lesson);
    lesson->setParent(this);
    const int index = m_lessons.length() - 1;
    updateLessonCharacters(index);
    connect(lesson, SIGNAL(newCharactersChanged()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(lesson, index);
    emit lessonCountChanged();
}

void Course::removeLesson(int index)
{
    Q_ASSERT(index >= 0 && index < m_lessons.count());
    Lesson* const lesson = m_lessons.at(index);
    m_lessons.removeAt(index);
    m_signalMapper->removeMappings(lesson);
    delete lesson;
    updateLessonCharacters(index);
    emit lessonCountChanged();
}

void Course::clearLessons()
{
    for (int i = 0; i < lessonCount(); i++)
    {
        m_signalMapper->removeMappings(lesson(i));
    }
    qDeleteAll(m_lessons);
    m_lessons.clear();
    emit lessonCountChanged();
}

void Course::copyFrom(Course* source)
{
    setIsValid(false);
    setId(source->id());
    setTitle(source->title());
    setDescription(source->description());
    setKeyboardLayoutName(source->keyboardLayoutName());
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
