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
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import ktouch 1.0

Item {
    id: root

    property CategorizedResourceSortFilterProxyModel courseModel
    property Profile profile
    property KeyboardLayout keyboardLayout
    property string keyboardLayoutName

    signal lessonSelected(variant course, variant lesson)

    function selectLastUsedCourse() {
        if (!profile) {
            return
        }

        var courseId = profile.lastUsedCourseId;

        if (courseId === "custom_lessons") {
            selectCourse(courseRepeater.count, true)
            return
        }

        for (var i = 0; i < courseModel.rowCount(); i++) {
            var dataIndexCourse = courseModel.data(courseModel.index(i, 0), ResourceModel.DataRole);
            if (dataIndexCourse.id === courseId) {
                selectCourse(i, true)
                return
            }
        }

        selectCourse(0, true)
    }

    function selectCourse(index, automaticSelection) {
        if (index === priv.currentIndex) {
            return
        }

        var direction = index > priv.currentIndex? Item.Left: Item.Right
        var dataIndexCourse = index < courseModel.rowCount()?
                courseModel.data(courseModel.index(index, 0), ResourceModel.DataRole):
                null;
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
            nextButton.visible = previousButton.visible = courseModel.rowCount() > 1
            priv.currentIndex = -1
            selectLastUsedCourse()

        }
        onRowsInserted: {
            nextButton.visible = previousButton.visible = courseModel.rowCount() > 1
            priv.currentIndex = -1
            selectLastUsedCourse()
        }
    }

    QtObject {
        id: priv
        property int currentIndex: -1
    }

    SystemPalette {
        id: palette
        colorGroup: SystemPalette.Active
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: head
            Layout.fillWidth: true
            height: Math.ceil(Math.max(courseTitleLabel.height, courseDescriptionButton.height) + 6)
            color: palette.base

            RowLayout {
                anchors {
                    fill: parent
                    leftMargin: 5
                    rightMargin: 5
                    topMargin: 3
                    bottomMargin: 3
                }

                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    id: courseTitleLabel
                    font.pointSize: 1.5 * Qt.font({'family': 'sansserif'}).pointSize
                    text: coursePageContainer.activePage.course.title
                }

                Item {
                    id: smallSpacer
                    height: parent.height
                    width: 3
                }

                ToolButton {
                    id: courseDescriptionButton
                    anchors.verticalCenter: parent.verticalCenter
                    iconName: "dialog-information"
                    checkable: true
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                ToolButton {
                    id: previousButton
                    anchors.verticalCenter: parent.verticalCenter
                    iconName: "arrow-left"
                    enabled: priv.currentIndex > 0
                    onClicked: {
                        var newIndex = priv.currentIndex - 1
                        root.selectCourse(newIndex, false)
                    }
                }

                ToolButton {
                    id: nextButton
                    iconName: "arrow-right"
                    enabled: priv.currentIndex < courseModel.rowCount()
                    onClicked: {
                        var newIndex = (priv.currentIndex + 1) % (courseModel.rowCount() + 1)
                        root.selectCourse(newIndex, false)
                    }
                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.minimumHeight: courseDescriptionItem.height
            Layout.maximumHeight: courseDescriptionItem.height
            CourseDescriptionItem {
                id: courseDescriptionItem
                active: courseDescriptionButton.checked
                description: coursePageContainer.activePage.course.description
                width: parent.width
            }
        }

        Item {
            id: coursePageContainer
            property CoursePage activePage: page0
            property CoursePage inactivePage: page1

            Layout.fillWidth: true
            Layout.fillHeight: true

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
