/*
 * SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
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
