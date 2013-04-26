/*
 *  Copyright 2013  Sebastian Gottfried <sebastiangottfried@web.de>
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

#include "customlessonmodel.h"

#include <KLocale>

#include "core/profile.h"
#include "core/profiledataaccess.h"

CustomLessonModel::CustomLessonModel(QObject* parent) :
    QSqlQueryModel(parent),
    m_profile(0)
{
}

Profile* CustomLessonModel::profile() const
{
    return m_profile;
}

void CustomLessonModel::setProfile(Profile* profile)
{
    if (profile != m_profile)
    {
        if (m_profile)
        {
            m_profile->disconnect(this);
        }

        m_profile = profile;

        if (m_profile)
        {
            connect(m_profile, SIGNAL(idChanged()), SLOT(update()));
            connect(m_profile, SIGNAL(destroyed()), SLOT(profileDestroyed()));
        }

        update();
        emit profileChanged();
    }
}

QString CustomLessonModel::keyboardLayoutNameFilter() const
{
    return m_keyboardLayoutNameFilter;
}

void CustomLessonModel::setKeyboardLayoutNameFilter(const QString& name)
{
    if (name != m_keyboardLayoutNameFilter)
    {
        m_keyboardLayoutNameFilter = name;
        update();
        emit keyboardLayoutNameFilterChanged();
    }
}


void CustomLessonModel::update()
{
    ProfileDataAccess access;

    clear();

    if (m_profile)
    {
        const QSqlQuery query = access.customLessonQuery(m_profile, m_keyboardLayoutNameFilter);
        setQuery(query);
    }

}

void CustomLessonModel::profileDestroyed()
{
    setProfile(0);
}
