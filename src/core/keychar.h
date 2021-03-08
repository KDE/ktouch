/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef KEYCHAR_H
#define KEYCHAR_H

#include <QObject>

#include <QChar>
#include <QString>

class KeyChar : public QObject
{
    Q_OBJECT
    Q_ENUMS(Position)
    Q_PROPERTY(QChar value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(Position position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QString modifier READ modifier WRITE setModifier NOTIFY modifierChanged)

public:
    enum Position
    {
        Hidden,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    explicit KeyChar(QObject *parent = 0);
    QString positionStr() const;
    void setPositionStr(const QString& positionStr);
    QChar value() const;
    void setValue(const QChar& value);
    Position position() const;
    void setPosition(Position position);
    QString modifier() const;
    void setModifier(const QString& modifier);
    Q_INVOKABLE void copyFrom(KeyChar* source);

signals:
    void valueChanged();
    void positionChanged();
    void modifierChanged();

private:
    QChar m_value;
    Position m_position;
    QString m_modifier;
};

#endif // KEYCHAR_H
