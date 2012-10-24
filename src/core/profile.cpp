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

void Profile::setLastUsedCourseId(QString id)
{
    if (id != m_lastUsedCourseId)
    {
        m_lastUsedCourseId = id;
        emit lastUsedCourseIdChanged();
    }
}
