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

#include "specialkey.h"

SpecialKey::SpecialKey(QObject *parent) :
    AbstractKey(parent),
    m_type(SpecialKey::Other)
{
}

QString SpecialKey::typeStr() const
{
    switch(m_type)
    {
    case SpecialKey::Tab:
        return "tab";
    case SpecialKey::Capslock:
        return "capslock";
    case SpecialKey::Shift:
        return "shift";
    case SpecialKey::Backspace:
        return "backspace";
    case SpecialKey::Return:
        return "return";
    case SpecialKey::Space:
        return "space";
    case SpecialKey::Other:
    default:
        return "other";
    }
}

void SpecialKey::setTypeStr(const QString &typeStr)
{
    if (typeStr == "tab")
    {
        m_type = SpecialKey::Tab;
    }
    else if (typeStr == "capslock")
    {
        m_type = SpecialKey::Capslock;
    }
    else if (typeStr == "shift")
    {
        m_type = SpecialKey::Shift;
    }
    else if (typeStr == "backspace")
    {
        m_type = SpecialKey::Backspace;
    }
    else if (typeStr == "return")
    {
        m_type = SpecialKey::Return;
    }
    else if (typeStr == "space")
    {
        m_type = SpecialKey::Space;
    }
    else
    {
        m_type = SpecialKey::Other;
    }
}
