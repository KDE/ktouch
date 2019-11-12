/*
 * Copyright 2017  Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "lessontexthighlighteritem.h"

#include <QQuickTextDocument>

#include "editor/lessontexthighlighter.h"

LessonTextHighlighterItem::LessonTextHighlighterItem():
    m_highligher(new LessonTextHighlighter(this))
{
}

QString LessonTextHighlighterItem::allowedCharacters() const
{
    return m_highligher->allowedCharacters();
}

void LessonTextHighlighterItem::setAllowedCharacters(const QString& characters)
{
    if (characters != m_highligher->allowedCharacters())
    {
        m_highligher->setAllowedCharacters(characters);
        emit allowedCharactersChanged();
    }
}

QQuickTextDocument* LessonTextHighlighterItem::document() const
{
    return m_document;
}

void LessonTextHighlighterItem::setDocument(QQuickTextDocument* document)
{
    if (document != m_document)
    {
        m_document = document;
        if (document != nullptr)
        {
            m_highligher->setDocument(document->textDocument());
        }
        else
        {
            m_highligher->setDocument(nullptr);
        }
        emit documentChanged();
    }
}
