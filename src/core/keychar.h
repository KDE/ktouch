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

    QChar value() const
    {
        return m_value;
    }

    void setValue(const QChar& value)
    {
        if (value != m_value)
        {
            m_value = value;
            emit valueChanged(value);
        }
    }

    Position position() const
    {
        return m_position;
    }

    void setPosition(Position position)
    {
        if (position != m_position)
        {
            m_position = position;
            emit positionChanged(position);
        }
    }

    QString modifier() const
    {
        return m_modifier;
    }

    void setModifier(const QString& modifier)
    {
        if (modifier != m_modifier)
        {
            m_modifier = modifier;
            emit modifierChanged(modifier);
        }
    }

signals:
    void valueChanged(const QChar& newValue);
    void positionChanged(Position newPosition);
    void modifierChanged(const QString& newModifier);

private:
    QChar m_value;
    Position m_position;
    QString m_modifier;
};

#endif // KEYCHAR_H
