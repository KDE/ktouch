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
    signal lessonSelected(variant course, int lessonIndex)

    function selectLastUsedCourse() {
        if (!profile)
            return
        var courseId = profile.lastUsedCourseId;
        for (var i = 0; i < courseRepeater.count; i++) {
            var dataIndexCourse = courseRepeater.itemAt(i).dataIndexCourse
            if (dataIndexCourse.id === courseId) {
                selectCourse(i, true)
                return
            }
        }
        if (courseRepeater.count > 0) {
            selectCourse(0, true)
        }
    }

    function selectCourse(index, automaticSelection) {
        if (index === priv.currentIndex)
            return

        var direction = index > priv.currentIndex? Item.Left: Item.Right
        var dataIndexCourse = courseRepeater.itemAt(index).dataIndexCourse
        var targetPage = automaticSelection? coursePageContainer.activePage: coursePageContainer.inactivePage

        priv.currentIndex = index;
        targetPage.dataIndexCourse = dataIndexCourse
        courseTitleLabel.text = dataIndexCourse.title
        courseDescriptionItem.description = dataIndexCourse.description

        if (!automaticSelection) {
            coursePageContainer.inactivePage = coursePageContainer.activePage
            coursePageContainer.activePage = targetPage
            coursePageContainer.inactivePage.hide(direction)
            coursePageContainer.activePage.show(direction)

            saveLastUsedCourse(dataIndexCourse.id)
        }
    }

    function saveLastUsedCourse(courseId) {
        profile.lastUsedCourseId = courseId;
        profileDataAccess.updateProfile(profileDataAccess.indexOfProfile(profile));
    }

    onProfileChanged: selectLastUsedCourse()

    Connections {
        target: courseModel
        onLayoutChanged: selectLastUsedCourse()
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
        visible: courseRepeater.count > 0

        Rectangle {
            id: head
            width: parent.width
            height: Math.ceil(courseTitleLabel.height + 6)
            color: "#fff"

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
                    visible: courseRepeater.count > 1
                    enabled: priv.currentIndex > 0
                    onClicked: {
                        var newIndex = priv.currentIndex - 1
                        root.selectCourse(newIndex, false)
                    }
                }

                PlasmaComponents.ToolButton {
                    id: nextButton
                    iconSource: "arrow-right"
                    visible: courseRepeater.count > 1
                    enabled: priv.currentIndex < courseRepeater.count - 1
                    onClicked: {
                        var newIndex = (priv.currentIndex + 1) % courseRepeater.count
                        root.selectCourse(newIndex, false)
                        saveLastUsedCourse
                    }
                }
            }
        }

        CourseDescriptionItem {
            id: courseDescriptionItem
            width: parent.width
            active: courseDescriptionButton.checked
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
                onLessonSelected: root.lessonSelected(course, lessonIndex)
                Component.onCompleted: page0.showImmediately()
            }

            CoursePage {
                id: page1
                profile: root.profile
                onLessonSelected: root.lessonSelected(course, lessonIndex)
            }
        }
    }

    CoursePage {
        profile: root.profile
    }

    NoCoursesMessage {
        visible: courseRepeater.count === 0
        anchors.fill: parent
    }
}
