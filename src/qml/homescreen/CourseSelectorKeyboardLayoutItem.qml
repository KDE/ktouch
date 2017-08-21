/*
 *  Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *  Copyright 2015  Sebastian Gottfried <sebastiangottfried@web.de>
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

import QtQuick 2.4
import ktouch 1.0

import "../common"

Column {
    id: root
    property string name
    property alias title: keyboardLayoutItem.text
    property ResourceModel resourceModel
    property string selectedKeyboardLayoutName
    property DataIndexCourse selectedCourse

    signal courseSelected(DataIndexCourse course)

    height: keyboardLayoutItem.height + (loader.active? loader.height: 0)
    clip: true

    onSelectedKeyboardLayoutNameChanged: {
        if (selectedKeyboardLayoutName == root.name) {
            loader.active = true
        }
    }

    CategorizedResourceSortFilterProxyModel {
        id: courseModel
        resourceModel: root.resourceModel
        resourceTypeFilter: ResourceModel.CourseItem
        keyboardLayoutNameFilter: loader.keyboardLayoutNameFilter
    }

    ListItem {
        id: keyboardLayoutItem
        icon: "input-keyboard"
        width: parent.width
        onClicked: {
            loader.active = !loader.active
            if (loader.active) {
                if (courseModel.rowCount()) {
                    courseSelected(courseModel.data(courseModel.index(0, 0), ResourceModel.DataRole))
                }
            }
        }
    }

    Loader {
        id: loader
        width: parent.width
        active: false
        property string keyboardLayoutNameFilter: root.name
        sourceComponent: Component {
            id: courseSelectionComponent

            Column {
                Repeater {
                    id: courseRepeater
                    model: courseModel
                    ListItem {
                        text: display
                        width: parent.width
                        reserveSpaceForIcon: true
                        highlighted: root.selectedCourse == dataRole
                        onClicked: {
                            courseSelected(dataRole)
                        }
                    }
                }
                ListItem {
                    DataIndexCourse {
                        id: customLessonsCourse
                        title: i18n("Custom Lessons")
                        keyboardLayoutName: root.name
                        Component.onCompleted: {
                            id = "custom_lessons"
                        }
                    }

                    text: customLessonsCourse.title
                    id: ownLessonsItem
                    reserveSpaceForIcon: true
                    width: parent.width
                    highlighted: root.selectedCourse == customLessonsCourse
                    onClicked: {
                        courseSelected(customLessonsCourse)
                    }
                }
            }
        }
    }

    Behavior on height {
        NumberAnimation {
            duration: 150
            easing.type: Easing.InOutQuad
        }
    }
}

