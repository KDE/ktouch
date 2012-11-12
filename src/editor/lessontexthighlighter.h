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

#ifndef LESSONTEXTHIGHLIGHTER_H
#define LESSONTEXTHIGHLIGHTER_H

#include <QSyntaxHighlighter>

#include <QTextCharFormat>

class LessonTextHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit LessonTextHighlighter(QObject* parent = 0);
    int maximumLineLength() const;
    void setMaximumLineLength(int length);
    QString allowedCharacters() const;
    void setAllowedCharacters(const QString& characters);

protected:
    virtual void highlightBlock(const QString& text);
private:
    int m_maximumLineLength;
    QString m_allowedCharacters;
    QTextCharFormat m_overLongLineFormat;
    QTextCharFormat m_invalidCharFormat;
};

#endif // LESSONTEXTHIGHLIGHTER_H
