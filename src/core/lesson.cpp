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

QString Lesson::newCharacters() const
{
    return m_newCharacters;
}

void Lesson::setNewCharacters(const QString& newCharacters)
{
    if (newCharacters != m_newCharacters)
    {
        m_newCharacters = newCharacters;
        emit newCharactersChanged();
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

QString Lesson::text()
{
    return m_text;
}

void Lesson::setText(const QString& text)
{
    if (text != m_text)
    {
        m_text = text;
        emit textChanged();
    }
}

void Lesson::copyFrom(Lesson* source)
{
    setId(source->id());
    setTitle(source->title());
    setNewCharacters(source->newCharacters());
    setCharacters(source->characters());
    setText(source->text());
}
