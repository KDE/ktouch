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
