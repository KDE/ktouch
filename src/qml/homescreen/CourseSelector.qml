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
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import ktouch 1.0

import "../common"

FocusScope {
    id: root

    property CategorizedResourceSortFilterProxyModel courseModel
    property Profile profile
    property KeyboardLayout keyboardLayout
    property string keyboardLayoutName
    property DataIndexCourse selectedCourse

    signal lessonSelected(variant course, variant lesson)
    signal courseSelectec(Course course)

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
            selectLastUsedCourse()

        }
        onRowsInserted: {
            nextButton.visible = previousButton.visible = courseModel.rowCount() > 1
            selectLastUsedCourse()
        }
    }

    ResourceModel {
        id: resourceModel
        dataIndex: ktouch.globalDataIndex
    }

    CategorizedResourceSortFilterProxyModel {
        id: allKeyboardLayoutsModel
        resourceModel: resourceModel
        resourceTypeFilter: ResourceModel.KeyboardLayoutItem
    }

    KColorScheme {
        id: courseSelectorColorScheme
        colorGroup: KColorScheme.Active
        colorSet: KColorScheme.View
    }

    Rectangle {
        id: bg
        anchors.fill: parent
        color: courseSelectorColorScheme.normalBackground
    }

    Flickable {
        clip: true
        anchors.fill: parent
        contentWidth: width
        contentHeight: content.height
        Column {
            id: content
            width: parent.width

            ListItem {
                width: parent.width
                text: i18n('Courses For Your Keyboard Layout')
                font.bold: true
                bg.color: courseSelectorColorScheme.alternateBackground
                bg.opacity: 1
                label.opacity: 0.7
            }

            ListItem {
                width: parent.width
                text: i18n('Other Courses')
                font.bold: true
                bg.color: courseSelectorColorScheme.alternateBackground
                bg.opacity: 1
                label.opacity: 0.7
            }

            Repeater {
                model: allKeyboardLayoutsModel
                CourseSelectorKeyboardLayoutItem {
                    width: parent.width
                    name: keyboardLayoutName
                    title: display
                    resourceModel: allKeyboardLayoutsModel.resourceModel
                    selectedCourse: root.selectedCourse
                    onCourseSelected: {
                        root.selectedCourse = course
                    }
                }
            }

        }
        ScrollBar.vertical: ScrollBar { }
    }






}
