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

#include "dataaccess.h"

#include "core/dataindex.h"
#include "core/resourcedataaccess.h"
#include "core/userdataaccess.h"

DataAccess::DataAccess(QObject* parent) :
    QObject(parent)
{
}

bool DataAccess::loadDataIndex(DataIndex* target)
{
    target->setIsValid(false);
    target->clearCourses();
    target->clearKeyboardLayouts();

    ResourceDataAccess resourceDataAccess;
    UserDataAccess userDataAccess;

    const bool valid = resourceDataAccess.fillDataIndex(target) && userDataAccess.fillDataIndex(target);

    target->setIsValid(valid);

    return valid;
}

bool DataAccess::loadCourse(DataIndexCourse* dataIndexCourse, Course* target)
{
    ResourceDataAccess resourceDataAccess;
    UserDataAccess userDataAccess;

    switch (dataIndexCourse->source())
    {
    case DataIndex::BuiltInResource:
        return resourceDataAccess.loadCourse(dataIndexCourse->path(), target);
    case DataIndex::UserResource:
        return userDataAccess.loadCourse(dataIndexCourse->id(), target);
    default:
        return true;
    }
}

bool DataAccess::loadKeyboardLayout(DataIndexKeyboardLayout* dataIndexKeyboardLayout, KeyboardLayout* target)
{
    ResourceDataAccess resourceDataAccess;
    UserDataAccess userDataAccess;

    switch (dataIndexKeyboardLayout->source())
    {
    case DataIndex::BuiltInResource:
        return resourceDataAccess.loadKeyboardLayout(dataIndexKeyboardLayout->path(), target);
    case DataIndex::UserResource:
        return userDataAccess.loadKeyboardLayout(dataIndexKeyboardLayout->id(), target);
    default:
        return true;
    }
}
