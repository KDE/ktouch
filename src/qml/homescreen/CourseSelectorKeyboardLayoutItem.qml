/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *  SPDX-FileCopyrightText: 2015 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
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
        iconName: "input-keyboard"
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
                        text: dataRole.title
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
            duration: Units.shortDuration
            easing.type: Easing.InOutQuad
        }
    }
}

