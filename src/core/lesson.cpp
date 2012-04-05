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

#include "lesson.h"

#include "lessonline.h"

Lesson::Lesson(QObject *parent) :
    QObject(parent)
{
}

LessonLine* Lesson::line(unsigned int index) const
{
    Q_ASSERT(index < m_lines.count());
    return m_lines.at(index);
}

void Lesson::addLine(LessonLine* line)
{
    m_lines.append(line);
    emit lineCountChanged(m_lines.count());
}

void Lesson::removeLine(unsigned int index)
{
    Q_ASSERT(index < m_lines.count());
    delete m_lines.at(index);
    m_lines.removeAt(index);
    emit lineCountChanged(m_lines.count());
}

void Lesson::clearLines()
{
    qDeleteAll(m_lines);
    m_lines.clear();
    emit lineCountChanged(m_lines.count());
}
