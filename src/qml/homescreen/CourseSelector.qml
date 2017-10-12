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

    property Profile profile
    property KeyboardLayout keyboardLayout
    property string currentKeyboardLayoutName
    property string selectedKeyboardLayoutName
    property DataIndexCourse selectedCourse

    signal lessonSelected(variant course, variant lesson)
    signal courseSelectec(Course course)

    function selectLastUsedCourse() {
        if (!profile) {
            return
        }

        var courseId = profile.lastUsedCourseId;

        for (var i = 0; i < allCoursesModel.rowCount(); i++) {
            var dataIndexCourse = allCoursesModel.data(allCoursesModel.index(i, 0), ResourceModel.DataRole);
            if (dataIndexCourse.id === courseId) {
                root.selectedCourse = dataIndexCourse
                return
            }
        }

        if (allCoursesModel.rowCount() > 0) {
            var dataIndexCourse = allCoursesModel.data(allCoursesModel.index(0, 0), ResourceModel.DataRole);
            root.selectedCourse = dataIndexCourse
        }
    }

    onSelectedCourseChanged: {
        root.selectedKeyboardLayoutName = root.selectedCourse.keyboardLayoutName;
    }

    function saveLastUsedCourse(course) {
        if (profile.lastUsedCourseId != course.id) {
            profile.lastUsedCourseId = course.id;
            profileDataAccess.updateProfile(profileDataAccess.indexOfProfile(profile));
        }
    }

    onProfileChanged: selectLastUsedCourse()

    ResourceModel {
        id: resourceModel
        dataIndex: ktouch.globalDataIndex
        onRowsRemoved: {
            selectLastUsedCourse()

        }
        onRowsInserted: {
            selectLastUsedCourse()
        }
    }

    CategorizedResourceSortFilterProxyModel {
        id: allCoursesModel
        resourceModel: resourceModel
        resourceTypeFilter: ResourceModel.CourseItem
    }

    CategorizedResourceSortFilterProxyModel {
        id: currentKeyboardLayoutsModel
        resourceModel: resourceModel
        resourceTypeFilter: ResourceModel.KeyboardLayoutItem
        keyboardLayoutNameFilter: root.currentKeyboardLayoutName
    }

    CategorizedResourceSortFilterProxyModel {
        id: otherKeyboardLayoutsModel
        resourceModel: resourceModel
        resourceTypeFilter: ResourceModel.KeyboardLayoutItem
        keyboardLayoutNameFilter: root.currentKeyboardLayoutName
        invertedKeyboardLayoutNameFilter: true
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

            CourseSelectorKeyboardLayoutList {
                width: parent.width
                title: i18n('Courses For Your Keyboard Layout')
                model: currentKeyboardLayoutsModel
                resourceModel: resourceModel
                colorScheme: courseSelectorColorScheme
                selectedKeyboardLayoutName: root.selectedKeyboardLayoutName
                selectedCourse: root.selectedCourse
                onCourseSelected: {
                    root.selectedCourse = course
                    root.saveLastUsedCourse(course)
                }
            }

            CourseSelectorKeyboardLayoutList {
                width: parent.width
                title: i18n('Other courses')
                model: otherKeyboardLayoutsModel
                resourceModel: resourceModel
                colorScheme: courseSelectorColorScheme
                selectedKeyboardLayoutName: root.selectedKeyboardLayoutName
                selectedCourse: root.selectedCourse
                onCourseSelected: {
                    root.selectedCourse = course
                    root.saveLastUsedCourse(course)
                }
            }
        }

        ScrollBar.vertical: ScrollBar { }
    }






}
