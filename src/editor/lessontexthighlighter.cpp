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

#include "lessontexthighlighter.h"

#include <KColorScheme>
#include <KDebug>

LessonTextHighlighter::LessonTextHighlighter(QObject* parent):
    QSyntaxHighlighter(parent)
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

    if (text.length() > 60)
    {
        setFormat(60, text.length() - 60, m_overLongLineFormat);
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
