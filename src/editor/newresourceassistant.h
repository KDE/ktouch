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

#ifndef NEWRESOURCEASSISTANT_H
#define NEWRESOURCEASSISTANT_H

#include <KAssistantDialog>

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
