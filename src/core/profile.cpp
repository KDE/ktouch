/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "profile.h"

Profile::Profile(QObject* parent) :
    QObject(parent),
    m_id(-1),
    m_skillLevel(Profile::Beginner)
{
}

int Profile::id() const
{
    return m_id;
}

void Profile::setId(int id)
{
    if (id != m_id)
    {
        m_id = id;
        emit idChanged();
    }
}

QString Profile::name() const
{
    return m_name;
}

void Profile::setName(const QString &name)
{
    if (name != m_name)
    {
        m_name = name;
        emit nameChanged();
    }
}

Profile::SkillLevel Profile::skillLevel() const
{
    return m_skillLevel;
}

void Profile::setSkillLevel(SkillLevel skillLevel)
{
    if (skillLevel != m_skillLevel)
    {
        m_skillLevel = skillLevel;
        emit skillLevelChanged();
    }
}

QString Profile::lastUsedCourseId() const
{
    return m_lastUsedCourseId;
}

void Profile::setLastUsedCourseId(const QString &id)
{
    if (id != m_lastUsedCourseId)
    {
        m_lastUsedCourseId = id;
        emit lastUsedCourseIdChanged();
    }
}
