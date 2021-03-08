/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "specialkey.h"

SpecialKey::SpecialKey(QObject *parent) :
    AbstractKey(parent),
    m_type(SpecialKey::Other)
{
}

QString SpecialKey::keyType() const
{
    return QStringLiteral("specialKey");
}

QString SpecialKey::typeStr() const
{
    switch(m_type)
    {
    case SpecialKey::Tab:
        return QStringLiteral("tab");
    case SpecialKey::Capslock:
        return QStringLiteral("capslock");
    case SpecialKey::Shift:
        return QStringLiteral("shift");
    case SpecialKey::Backspace:
        return QStringLiteral("backspace");
    case SpecialKey::Return:
        return QStringLiteral("return");
    case SpecialKey::Space:
        return QStringLiteral("space");
    case SpecialKey::Other:
    default:
        return QStringLiteral("other");
    }
}

void SpecialKey::setTypeStr(const QString &typeStr)
{
    if (typeStr == QLatin1String("tab"))
    {
        m_type = SpecialKey::Tab;
    }
    else if (typeStr == QLatin1String("capslock"))
    {
        m_type = SpecialKey::Capslock;
    }
    else if (typeStr == QLatin1String("shift"))
    {
        m_type = SpecialKey::Shift;
    }
    else if (typeStr == QLatin1String("backspace"))
    {
        m_type = SpecialKey::Backspace;
    }
    else if (typeStr == QLatin1String("return"))
    {
        m_type = SpecialKey::Return;
    }
    else if (typeStr == QLatin1String("space"))
    {
        m_type = SpecialKey::Space;
    }
    else
    {
        m_type = SpecialKey::Other;
    }
}

SpecialKey::Type SpecialKey::type() const
{
    return m_type;
}

void SpecialKey::setType(SpecialKey::Type type)
{
    if(type != m_type)
    {
        m_type = type;
        emit typeChanged();
    }
}

QString SpecialKey::modifierId() const
{
    return m_modifierId;
}

void SpecialKey::setModifierId(const QString& modifierId)
{
    if(modifierId != m_modifierId)
    {
        m_modifierId = modifierId;
        emit modifierIdChanged();
    }
}

QString SpecialKey::label() const
{
    return m_label;
}

void SpecialKey::setLabel(const QString& label)
{
    if(label != m_label)
    {
        m_label = label;
        emit labelChanged();
    }
}

void SpecialKey::copyFrom(SpecialKey* source)
{
    AbstractKey::copyFrom(source);
    setType(source->type());
    setLabel(source->label());
    setModifierId(source->modifierId());
}
