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

#include "keychar.h"

#include <kdebug.h>

KeyChar::KeyChar(QObject *parent) :
    QObject(parent),
    m_value(QChar(32)),
    m_position(KeyChar::Hidden)
{
}

QString KeyChar::positionStr() const
{
    switch (m_position)
    {
    case KeyChar::TopLeft:
        return "topLeft";
    case KeyChar::TopRight:
        return "topRight";
    case KeyChar::BottomLeft:
        return "bottomLeft";
    case KeyChar::BottomRight:
        return "bottomRight";
    case KeyChar::Hidden:
    default:
        return "hidden";
    }
}

void KeyChar::setPositionStr(const QString &position)
{
    if (position == "topLeft")
    {
        m_position = KeyChar::TopLeft;
    }
    else if (position == "topRight")
    {
        m_position = KeyChar::TopRight;
    }
    else if (position == "bottomLeft")
    {
        m_position = KeyChar::BottomLeft;
    }
    else if (position == "bottomRight")
    {
        m_position = KeyChar::BottomRight;
    }
    else
    {
        m_position = KeyChar::Hidden;
    }
}

QChar KeyChar::value() const
{
    return m_value;
}

void KeyChar::setValue(const QChar& value)
{
    if(value != m_value)
    {
        m_value = value;
        emit valueChanged();
    }
}

KeyChar::Position KeyChar::position() const
{
    return m_position;
}

void KeyChar::setPosition(KeyChar::Position position)
{
    if(position != m_position)
    {
        m_position = position;
        emit positionChanged();
    }
}

QString KeyChar::modifier() const
{
    return m_modifier;
}

void KeyChar::setModifier(const QString& modifier)
{
    if(modifier != m_modifier)
    {
        m_modifier = modifier;
        emit modifierChanged();
    }
}

void KeyChar::copyFrom(KeyChar* source)
{
    setValue(source->value());
    setPosition(source->position());
    setModifier(source->modifier());
}

