/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "keychar.h"

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
        return QStringLiteral("topLeft");
    case KeyChar::TopRight:
        return QStringLiteral("topRight");
    case KeyChar::BottomLeft:
        return QStringLiteral("bottomLeft");
    case KeyChar::BottomRight:
        return QStringLiteral("bottomRight");
    case KeyChar::Hidden:
    default:
        return QStringLiteral("hidden");
    }
}

void KeyChar::setPositionStr(const QString &position)
{
    if (position == QLatin1String("topLeft"))
    {
        m_position = KeyChar::TopLeft;
    }
    else if (position == QLatin1String("topRight"))
    {
        m_position = KeyChar::TopRight;
    }
    else if (position == QLatin1String("bottomLeft"))
    {
        m_position = KeyChar::BottomLeft;
    }
    else if (position == QLatin1String("bottomRight"))
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

