/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "keyboardlayoutbase.h"

KeyboardLayoutBase::KeyboardLayoutBase(QObject *parent) :
    Resource(parent)
{
}

const QString& KeyboardLayoutBase::title() const
{
    return m_title;
}

void KeyboardLayoutBase::setTitle(const QString& title)
{
    m_title = title;
    emit titleChanged();
}

const QString& KeyboardLayoutBase::name() const
{
    return m_name;
}

void KeyboardLayoutBase::setName(const QString& name)
{
    if(name != m_name)
    {
        m_name = name;
        emit nameChanged();
    }
}
