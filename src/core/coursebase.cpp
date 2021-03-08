/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
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
