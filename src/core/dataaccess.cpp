/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
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
