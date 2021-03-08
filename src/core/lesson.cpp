/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
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
