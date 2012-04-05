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

Lesson* Course::lesson(unsigned int index) const
{
    Q_ASSERT(index < m_lessons.count());
    return m_lessons.at(index);
}

void Course::addLesson(Lesson* lesson)
{
    m_lessons.append(lesson);
    lesson->setParent(this);
    emit lessonCountChanged(m_lessons.count());
}

void Course::removeLesson(unsigned int index)
{
    Q_ASSERT(index < m_lessons.count());
    delete m_lessons.at(index);
    m_lessons.removeAt(index);
    emit lessonCountChanged(m_lessons.count());
}

void Course::clearLessons()
{
    qDeleteAll(m_lessons);
    m_lessons.clear();
    emit lessonCountChanged(m_lessons.count());
}
