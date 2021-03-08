/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef NEWRESOURCEASSISTANT_H
#define NEWRESOURCEASSISTANT_H

#include <KAssistantDialog>
#include <KLocalizedString>

#include "models/resourcemodel.h"

class Resource;
class ResourceTypesWidget;
class NewCourseWidget;
class NewKeyboardLayoutWidget;
class ResourceTemplateWidget;

class NewResourceAssistant : public KAssistantDialog
{
    Q_OBJECT
public:
    explicit NewResourceAssistant(ResourceModel* resourceModel, QWidget* parent = 0);
    Resource* createResource();
private slots:
    void setResourceType(ResourceModel::ResourceItemType type);
    void updateNewCoursePageValidity();
    void updateNewKeyboardLayoutPageValidity();
    void updateResourceTemplatePageValidity();
private:
    ResourceModel* m_resourceModel;
    ResourceTypesWidget* m_resourceTypesWidget;
    NewCourseWidget* m_newCourseWidget;
    NewKeyboardLayoutWidget* m_newKeyboardLayoutWidget;
    ResourceTemplateWidget* m_resourceTemplateWidget;
    KPageWidgetItem* m_resourceTypesPage;
    KPageWidgetItem* m_newKeyboardLayoutPage;
    KPageWidgetItem* m_newCoursePage;
    KPageWidgetItem* m_resourceTemplatePage;
    ResourceModel::ResourceItemType m_resourceType;
};

#endif // NEWRESOURCEASSISTANT_H
