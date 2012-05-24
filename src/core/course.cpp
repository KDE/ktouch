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

#include <QFile>
#include <QUrl>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QXmlSchema>
#include <QXmlSchemaValidator>

#include <kstandarddirs.h>
#include <klocale.h>
#include <kdebug.h>

#include "lesson.h"

Course::Course(QObject *parent) :
    CourseBase(parent)
{
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
    emit lessonCountChanged();
}

void Course::removeLesson(int index)
{
    Q_ASSERT(index >= 0 && index < m_lessons.count());
    delete m_lessons.at(index);
    m_lessons.removeAt(index);
    emit lessonCountChanged();
}

void Course::clearLessons()
{
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
