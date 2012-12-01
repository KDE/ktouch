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

#include "newresourceassistant.h"

#include <QUuid>

#include "core/resource.h"
#include "core/course.h"
#include "core/keyboardlayout.h"
#include "core/dataindex.h"
#include "core/dataaccess.h"
#include "resourcetypeswidget.h"
#include "newcoursewidget.h"
#include "newkeyboardlayoutwidget.h"
#include "resourcetemplatewidget.h"

NewResourceAssistant::NewResourceAssistant(ResourceModel* resourceModel, QWidget* parent) :
    KAssistantDialog(parent),
    m_resourceModel(resourceModel),
    m_resourceTypesWidget(new ResourceTypesWidget(this)),
    m_newCourseWidget(new NewCourseWidget(m_resourceModel, this)),
    m_newKeyboardLayoutWidget(new NewKeyboardLayoutWidget(m_resourceModel, this)),
    m_resourceTemplateWidget(new ResourceTemplateWidget(m_resourceModel, this))
{
    setWindowTitle(i18n("New"));

    m_resourceTypesPage = addPage(m_resourceTypesWidget, i18n("New..."));
    setValid(m_resourceTypesPage, false);

    m_newCoursePage = addPage(m_newCourseWidget, i18n("New course"));
    setValid(m_newCoursePage, false);
    setAppropriate(m_newCoursePage, false);

    m_newKeyboardLayoutPage = addPage(m_newKeyboardLayoutWidget, i18n("New keyboard layout"));
    setValid(m_newKeyboardLayoutPage, false);
    setAppropriate(m_newKeyboardLayoutPage, false);

    m_resourceTemplatePage = addPage(m_resourceTemplateWidget, i18n("Template"));
    setValid(m_resourceTemplatePage, false);

    connect(m_resourceTypesWidget, SIGNAL(typeSelected(ResourceModel::ResourceItemType)), SLOT(setResourceType(ResourceModel::ResourceItemType)));
    connect(m_newCourseWidget, SIGNAL(isValidChanged()), SLOT(updateNewCoursePageValidity()));
    connect(m_newKeyboardLayoutWidget, SIGNAL(isValidChanged()), SLOT(updateNewKeyboardLayoutPageValidity()));
    connect(m_resourceTemplateWidget, SIGNAL(isValidChanged()), SLOT(updateResourceTemplatePageValidity()));
}

Resource* NewResourceAssistant::createResource()
{
    if (m_resourceType == ResourceModel::CourseItem)
    {
        Course* course = new Course();
        DataIndexCourse* templateDataIndexCourse = qobject_cast<DataIndexCourse*>(m_resourceTemplateWidget->templateResource());

        if (templateDataIndexCourse)
        {
            Course templateCourse;
            DataAccess dataAccess;
            dataAccess.loadCourse(templateDataIndexCourse, &templateCourse);
            course->copyFrom(&templateCourse);
        }

        course->setId(QUuid::createUuid().toString());
        course->setTitle(m_newCourseWidget->title());
        course->setKeyboardLayoutName(m_newCourseWidget->keyboardLayoutName());
        course->setDescription(m_newCourseWidget->description());

        return course;
    }
    else if (m_resourceType == ResourceModel::KeyboardLayoutItem)
    {
        KeyboardLayout* keyboardLayout = new KeyboardLayout();
        DataIndexKeyboardLayout* templateDataIndexKeyboardLayout = qobject_cast<DataIndexKeyboardLayout*>(m_resourceTemplateWidget->templateResource());

        if (templateDataIndexKeyboardLayout)
        {
            KeyboardLayout templateKeyboardLayout;
            DataAccess dataAccess;
            dataAccess.loadKeyboardLayout(templateDataIndexKeyboardLayout, &templateKeyboardLayout);
            keyboardLayout->copyFrom(&templateKeyboardLayout);
        }

        keyboardLayout->setId(QUuid::createUuid().toString());
        keyboardLayout->setName(m_newKeyboardLayoutWidget->name());
        keyboardLayout->setTitle(m_newKeyboardLayoutWidget->title());

        return keyboardLayout;
    }

    return 0;
}

void NewResourceAssistant::setResourceType(ResourceModel::ResourceItemType type)
{
    m_resourceType = type;
    setAppropriate(m_newCoursePage, type == ResourceModel::CourseItem);
    setAppropriate(m_newKeyboardLayoutPage, type == ResourceModel::KeyboardLayoutItem);
    m_resourceTemplateWidget->setTemplateType(type);
    setValid(m_resourceTypesPage, true);
}

void NewResourceAssistant::updateNewCoursePageValidity()
{
    setValid(m_newCoursePage, m_newCourseWidget->isValid());
}

void NewResourceAssistant::updateNewKeyboardLayoutPageValidity()
{
    setValid(m_newKeyboardLayoutPage, m_newKeyboardLayoutWidget->isValid());
}

void NewResourceAssistant::updateResourceTemplatePageValidity()
{
    setValid(m_resourceTemplatePage, m_resourceTemplateWidget->isValid());
}
