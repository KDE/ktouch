/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import ktouch 1.0
import '../common'

Column {
    id: root
    property alias title: header.text
    property CategorizedResourceSortFilterProxyModel model
    property ResourceModel resourceModel
    property string selectedKeyboardLayoutName
    property DataIndexCourse selectedCourse: null
    property KColorScheme colorScheme
    signal courseSelected(DataIndexCourse course)

    ListItem {
        id: header
        width: parent.width
        font.bold: true
        bg.color: colorScheme.alternateBackground
        bg.opacity: 1
        label.opacity: 0.7
    }

    Repeater {
        id: repeater
        model: root.model
        CourseSelectorKeyboardLayoutItem {
            width: parent.width
            name: keyboardLayoutName
            title: display
            resourceModel: root.resourceModel
            selectedKeyboardLayoutName: root.selectedKeyboardLayoutName
            selectedCourse: root.selectedCourse
            onCourseSelected: {
                root.courseSelected(course)
            }
        }
    }

}
