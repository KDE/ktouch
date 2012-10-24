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

#include "coursebase.h"

CourseBase::CourseBase(QObject* parent) :
    Resource(parent)
{
}

QString CourseBase::title() const
{
    return m_title;
}

void CourseBase::setTitle(const QString& title)
{
    if(title != m_title)
    {
        m_title = title;
        emit titleChanged();
    }
}

QString CourseBase::description() const
{
    return m_description;
}

void CourseBase::setDescription(const QString& description)
{
    if(description != m_description)
    {
        m_description = description;
        emit descriptionChanged();
    }
}

QString CourseBase::keyboardLayoutName() const
{
    return m_keyboardLayoutName;
}

void CourseBase::setKeyboardLayoutName(const QString& keyboardLayoutName)
{
    if(keyboardLayoutName != m_keyboardLayoutName)
    {
        m_keyboardLayoutName = keyboardLayoutName;
        emit keyboardLayoutNameChanged();
    }
}
