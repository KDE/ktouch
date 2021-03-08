/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "resource.h"

Resource::Resource(QObject* parent):
    QObject(parent),
    m_isValid(false)
{
}

QString Resource::id() const
{
    return m_id;
}

void Resource::setId(const QString& id)
{
    if (id != m_id)
    {
        m_id = id;
        emit idChanged();
    }
}

bool Resource::isValid() const
{
    return m_isValid;
}

void Resource::setIsValid(bool isValid)
{
    if (isValid != m_isValid)
    {
        m_isValid = isValid;
        emit isValidChanged();
    }
}
