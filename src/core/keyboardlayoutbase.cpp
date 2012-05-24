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
