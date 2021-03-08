/*
 * SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
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
