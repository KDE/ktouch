/*
 *  Copyright 2017  Sebastian Gottfried <sebastian.gottfried@posteo.de>
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
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
