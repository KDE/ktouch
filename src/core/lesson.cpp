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

QString Lesson::id() const
{
    return m_id;
}

void Lesson::setId(const QString& id)
{
    if(id != m_id)
    {
        m_id = id;
        emit idChanged();
    }
}

QString Lesson::title() const
{
    return m_title;
}

void Lesson::setTitle(const QString& title)
{
    if(title != m_title)
    {
        m_title = title;
        emit titleChanged();
    }
}

QString Lesson::characters() const
{
    return m_characters;
}

void Lesson::setCharacters(const QString& characters)
{
    if(characters != m_characters)
    {
        m_characters = characters;
        emit charactersChanged();
    }
}

int Lesson::lineCount() const
{
    return m_lines.count();
}

LessonLine* Lesson::line(int index) const
{
    Q_ASSERT(index >= 0 && index < m_lines.count());
    return m_lines.at(index);
}

void Lesson::addLine(LessonLine* line)
{
    m_lines.append(line);
    emit lineCountChanged();
}

void Lesson::removeLine(int index)
{
    Q_ASSERT(index >= 0 && index < m_lines.count());
    delete m_lines.at(index);
    m_lines.removeAt(index);
    emit lineCountChanged();
}

void Lesson::clearLines()
{
    qDeleteAll(m_lines);
    m_lines.clear();
    emit lineCountChanged();
}

void Lesson::copyFrom(Lesson* source)
{
    setId(source->id());
    setTitle(source->title());
    setCharacters(source->characters());
    clearLines();
    for (int i = 0; i < source->lineCount(); i++)
    {
        LessonLine* lessonLine = new LessonLine(this);
        lessonLine->copyFrom(source->line(i));
        addLine(lessonLine);
    }
}
