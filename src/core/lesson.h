/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef LESSON_H
#define LESSON_H

#include <QObject>
#include <QString>
#include <QList>

class Lesson : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString newCharacters READ newCharacters WRITE setNewCharacters NOTIFY newCharactersChanged)
    Q_PROPERTY(QString characters READ characters WRITE setCharacters NOTIFY charactersChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
    explicit Lesson(QObject *parent = 0);
    QString id() const;
    void setId(const QString& id);
    QString title() const;
    void setTitle(const QString& title);
    QString newCharacters() const;
    void setNewCharacters(const QString& newCharacters);
    QString characters() const;
    void setCharacters(const QString& characters);
    QString text();
    void setText(const QString& text);
    Q_INVOKABLE void copyFrom(Lesson* source);

signals:
    void idChanged();
    void titleChanged();
    void newCharactersChanged();
    void charactersChanged();
    void textChanged();

private:
    Q_DISABLE_COPY(Lesson)
    QString m_id;
    QString m_title;
    QString m_newCharacters;
    QString m_characters;
    QString m_text;
};

#endif // LESSON_H
