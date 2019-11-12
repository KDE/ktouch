/*
 * Copyright 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
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

#ifndef LESSONTEXTHIGHLIGHTERITEM_H
#define LESSONTEXTHIGHLIGHTERITEM_H

#include <QQuickItem>

class LessonTextHighlighter;
class QQuickTextDocument;

class LessonTextHighlighterItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString allowedCharacters READ allowedCharacters WRITE setAllowedCharacters NOTIFY allowedCharactersChanged)
    Q_PROPERTY(QQuickTextDocument* document READ document WRITE setDocument NOTIFY documentChanged)
public:
    LessonTextHighlighterItem();
    QString allowedCharacters() const;
    void setAllowedCharacters(const QString& characters);
    QQuickTextDocument* document() const;
    void setDocument(QQuickTextDocument* document);
signals:
    void allowedCharactersChanged();
    void documentChanged();
private:
    LessonTextHighlighter* m_highligher;
    QQuickTextDocument* m_document;
};

#endif // LESSONTEXTHIGHLIGHTERITEM_H
