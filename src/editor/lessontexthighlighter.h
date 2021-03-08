/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef LESSONTEXTHIGHLIGHTER_H
#define LESSONTEXTHIGHLIGHTER_H

#include <QSyntaxHighlighter>

#include <QTextCharFormat>

class LessonTextHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit LessonTextHighlighter(QObject* parent = nullptr);
    int maximumLineLength() const;
    void setMaximumLineLength(int length);
    QString allowedCharacters() const;
    void setAllowedCharacters(const QString& characters);

protected:
    void highlightBlock(const QString& text) override;
private:
    int m_maximumLineLength;
    QString m_allowedCharacters;
    QTextCharFormat m_overLongLineFormat;
    QTextCharFormat m_invalidCharFormat;
};

#endif // LESSONTEXTHIGHLIGHTER_H
