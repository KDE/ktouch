/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "newresourceassistant.h"

#include <QUuid>

#include "core/resource.h"
#include "core/course.h"
#include "core/lesson.h"
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
    setWindowTitle(i18nc("@title:window", "New"));

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

    connect(m_resourceTypesWidget, &ResourceTypesWidget::typeSelected, this, &NewResourceAssistant::setResourceType);
    connect(m_newCourseWidget, &NewCourseWidget::isValidChanged, this, &NewResourceAssistant::updateNewCoursePageValidity);
    connect(m_newKeyboardLayoutWidget, &NewKeyboardLayoutWidget::isValidChanged, this, &NewResourceAssistant::updateNewKeyboardLayoutPageValidity);
    connect(m_resourceTemplateWidget, &ResourceTemplateWidget::isValidChanged, this, &NewResourceAssistant::updateResourceTemplatePageValidity);
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
            for (int i = 0; i < course->lessonCount(); i++)
            {
                course->lesson(i)->setId(QUuid::createUuid().toString());
            }
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
