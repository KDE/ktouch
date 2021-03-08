/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "lessontexthighlighter.h"

#include <KColorScheme>

LessonTextHighlighter::LessonTextHighlighter(QObject* parent):
    QSyntaxHighlighter(parent),
    m_maximumLineLength(60)
{
    KColorScheme inactiveScheme(QPalette::Inactive, KColorScheme::View);
    KColorScheme activeScheme(QPalette::Active, KColorScheme::View);
    m_overLongLineFormat.setForeground(inactiveScheme.foreground(KColorScheme::InactiveText));
    m_overLongLineFormat.setBackground(inactiveScheme.background(KColorScheme::NegativeBackground));
    m_invalidCharFormat.setForeground(activeScheme.background(KColorScheme::NeutralBackground));
    m_invalidCharFormat.setBackground(activeScheme.foreground(KColorScheme::NegativeText));
    m_invalidCharFormat.setFontWeight(QFont::Bold);
}

QString LessonTextHighlighter::allowedCharacters() const
{
    return m_allowedCharacters;
}

void LessonTextHighlighter::setAllowedCharacters(const QString& characters)
{
    if (characters != m_allowedCharacters)
    {
        m_allowedCharacters = characters;
        rehighlight();
    };
}

int LessonTextHighlighter::maximumLineLength() const
{
    return m_maximumLineLength;
}

void LessonTextHighlighter::setMaximumLineLength(int length)
{
    if (length != m_maximumLineLength)
    {
        m_maximumLineLength = length;
        rehighlight();
    }
}

void LessonTextHighlighter::highlightBlock(const QString& text)
{
    const QLatin1Char space(32);

    if (text.length() > m_maximumLineLength)
    {
        setFormat(m_maximumLineLength, text.length() - m_maximumLineLength, m_overLongLineFormat);
    }

    if (m_allowedCharacters.isNull())
        return;

    for (int i = 0; i < text.length(); i++)
    {
        const QChar c = text.at(i);

        if (c != space && !m_allowedCharacters.contains(c))
        {
            setFormat(i, 1, m_invalidCharFormat);
        }
    }
}
