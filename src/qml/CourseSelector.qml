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

import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import ktouch 1.0

Item {
    id: root

    property CategorizedResourceSortFilterProxyModel courseModel
    property Profile profile
    property KeyboardLayout keyboardLayout
    property string keyboardLayoutName

    signal lessonSelected(variant course, variant lesson)

    function selectLastUsedCourse() {
        if (!profile)
            return
        var courseId = profile.lastUsedCourseId;

        if (courseId == "custom_lessons") {
            selectCourse(courseRepeater.count, true)
            return
        }

        for (var i = 0; i < courseRepeater.count; i++) {
            var dataIndexCourse = courseRepeater.itemAt(i).dataIndexCourse
            if (dataIndexCourse.id === courseId) {
                selectCourse(i, true)
                return
            }
        }

        selectCourse(0, true)
    }

    function selectCourse(index, automaticSelection) {
        if (index === priv.currentIndex)
            return

        var direction = index > priv.currentIndex? Item.Left: Item.Right
        var dataIndexCourse = index < courseRepeater.count?
                courseRepeater.itemAt(index).dataIndexCourse: null
        var targetPage = automaticSelection? coursePageContainer.activePage: coursePageContainer.inactivePage

        priv.currentIndex = index;
        targetPage.dataIndexCourse = dataIndexCourse

        if (!automaticSelection) {
            coursePageContainer.inactivePage = coursePageContainer.activePage
            coursePageContainer.activePage = targetPage
            coursePageContainer.inactivePage.hide(direction)
            coursePageContainer.activePage.show(direction)

            saveLastUsedCourse(dataIndexCourse? dataIndexCourse.id: "custom_lessons")
        }
    }

    function saveLastUsedCourse(courseId) {
        profile.lastUsedCourseId = courseId;
        profileDataAccess.updateProfile(profileDataAccess.indexOfProfile(profile));
    }

    onProfileChanged: selectLastUsedCourse()

    Connections {
        target: courseModel
        onRowsRemoved: {
            priv.currentIndex = -1
            selectLastUsedCourse()

        }
        onRowsInserted: {
            priv.currentIndex = -1
            selectLastUsedCourse()
        }
    }

    Repeater {
        id: courseRepeater
        model: courseModel

        delegate: Item {
            property DataIndexCourse dataIndexCourse: dataRole
        }
    }

    QtObject {
        id: priv
        property int currentIndex: -1
    }

    Column {
        anchors.fill: parent

        Rectangle {
            id: head
            width: parent.width
            height: Math.ceil(courseTitleLabel.height + 6)
            color: theme.backgroundColor

            Row {
                anchors {
                    fill: parent
                    leftMargin: 5
                    rightMargin: 5
                    topMargin: 3
                    bottomMargin: 3
                }

                PlasmaComponents.Label {
                    id: courseTitleLabel
                    height: paintedHeight
                    font.pointSize: 1.5 * theme.defaultFont.pointSize
                    text: coursePageContainer.activePage.course.title
                }

                Item {
                    id: smallSpacer
                    height: parent.height
                    width: 3
                }

                PlasmaComponents.ToolButton {
                    id: courseDescriptionButton
                    iconSource: "dialog-information"
                    checkable: true
                }

                Item {
                    width: parent.width - courseTitleLabel.width - smallSpacer.width - courseDescriptionButton.width - previousButton.width - nextButton.width - (parent.children.length - 1) * parent.spacing
                    height: parent.height
                }

                PlasmaComponents.ToolButton {
                    id: previousButton
                    iconSource: "arrow-left"
                    enabled: priv.currentIndex > 0
                    visible: courseRepeater.count > 0
                    onClicked: {
                        var newIndex = priv.currentIndex - 1
                        root.selectCourse(newIndex, false)
                    }
                }

                PlasmaComponents.ToolButton {
                    id: nextButton
                    iconSource: "arrow-right"
                    enabled: priv.currentIndex < courseRepeater.count
                    visible: courseRepeater.count > 0
                    onClicked: {
                        var newIndex = (priv.currentIndex + 1) % (courseRepeater.count + 1)
                        root.selectCourse(newIndex, false)
                    }
                }
            }
        }

        CourseDescriptionItem {
            id: courseDescriptionItem
            width: parent.width
            active: courseDescriptionButton.checked
            description: coursePageContainer.activePage.course.description
        }

        Item {
            id: coursePageContainer
            property CoursePage activePage: page0
            property CoursePage inactivePage: page1

            width: parent.width
            height: parent.height - head.height - courseDescriptionItem.height

            CoursePage {
                id: page0
                profile: root.profile
                keyboardLayout: root.keyboardLayout
                keyboardLayoutName: root.keyboardLayoutName
                onLessonSelected: root.lessonSelected(course, lesson)
                Component.onCompleted: page0.showImmediately()
            }

            CoursePage {
                id: page1
                profile: root.profile
                keyboardLayout: root.keyboardLayout
                keyboardLayoutName: root.keyboardLayoutName
                onLessonSelected: root.lessonSelected(course, lesson)
            }
        }
    }
}
