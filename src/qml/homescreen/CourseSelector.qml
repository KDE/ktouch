/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *  SPDX-FileCopyrightText: 2015 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2 as Controls
import ktouch 1.0

import "../common"

FocusScope {
    id: root

    property Profile profile
    property string currentKeyboardLayoutName
    property string selectedKeyboardLayoutName
    property DataIndexKeyboardLayout selectedKeyboardLayout
    property DataIndexCourse selectedCourse

    signal lessonSelected(variant course, variant lesson)
    signal courseSelectec(Course course)

    function selectLastUsedCourse() {
        if (!profile) {
            return
        }

        var courseId = profile.lastUsedCourseId;

        // fist try to to select the course the user has used last
        for (var i = 0; i < allCoursesModel.rowCount(); i++) {
            var dataIndexCourse = allCoursesModel.data(allCoursesModel.index(i, 0), ResourceModel.DataRole);
            if (dataIndexCourse.id === courseId) {
                root.selectedCourse = dataIndexCourse
                return
            }
        }

        // if this fails try to select course matching the current keyboard layout
        if (coursesForCurrentKeyboardLayoutModel.rowCount() > 0) {
            var course = coursesForCurrentKeyboardLayoutModel.data(coursesForCurrentKeyboardLayoutModel.index(0, 0), ResourceModel.DataRole);
            root.selectedCourse = course
            return;
        }

        // finally just select the first course
        if (allCoursesModel.rowCount() > 0) {
            root.selectedCourse = allCoursesModel.data(allCoursesModel.index(0, 0), ResourceModel.DataRole);
        }
    }

    onSelectedCourseChanged: {
        root.selectedKeyboardLayoutName = root.selectedCourse.keyboardLayoutName;

        for (var i = 0; i < ktouch.globalDataIndex.keyboardLayoutCount; i++)
        {
            var dataIndexLayout = ktouch.globalDataIndex.keyboardLayout(i)

            if (dataIndexLayout.name === root.selectedKeyboardLayoutName) {
                root.selectedKeyboardLayout = dataIndexLayout;
                return
            }
        }

        root.selectedKeyboardLayout = null;
    }

    function saveLastUsedCourse(course) {
        if (profile.lastUsedCourseId != course.id) {
            profile.lastUsedCourseId = course.id;
            profileDataAccess.updateProfile(profileDataAccess.indexOfProfile(profile));
        }
    }

    onProfileChanged: selectLastUsedCourse()

    ResourceModel {
        id: allResourcesModel
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
        resourceModel: allResourcesModel
        resourceTypeFilter: ResourceModel.CourseItem
    }


    CategorizedResourceSortFilterProxyModel {
        id: coursesForCurrentKeyboardLayoutModel
        resourceModel: allResourcesModel
        resourceTypeFilter: ResourceModel.CourseItem
        keyboardLayoutNameFilter: root.currentKeyboardLayoutName
    }

    CategorizedResourceSortFilterProxyModel {
        id: currentKeyboardLayoutsModel
        resourceModel: allResourcesModel
        resourceTypeFilter: ResourceModel.KeyboardLayoutItem
        keyboardLayoutNameFilter: root.currentKeyboardLayoutName
    }

    CategorizedResourceSortFilterProxyModel {
        id: otherKeyboardLayoutsModel
        resourceModel: allResourcesModel
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

            Loader {
                width: parent.width
                active: root.currentKeyboardLayoutName != 'unknown'
                sourceComponent: CourseSelectorKeyboardLayoutList {
                    width: parent.width
                    title: i18n("Courses For Your Keyboard Layout")
                    model: currentKeyboardLayoutsModel
                    resourceModel: allResourcesModel
                    colorScheme: courseSelectorColorScheme
                    selectedKeyboardLayoutName: root.selectedKeyboardLayoutName
                    selectedCourse: root.selectedCourse
                    onCourseSelected: {
                        root.selectedCourse = course
                        root.saveLastUsedCourse(course)
                    }
                }
            }

            CourseSelectorKeyboardLayoutList {
                width: parent.width
                title: root.currentKeyboardLayoutName == 'unknown'? i18n("Courses"): i18n("Other Courses")
                model: otherKeyboardLayoutsModel
                resourceModel: allResourcesModel
                colorScheme: courseSelectorColorScheme
                selectedKeyboardLayoutName: root.selectedKeyboardLayoutName
                selectedCourse: root.selectedCourse
                onCourseSelected: {
                    root.selectedCourse = course
                    root.saveLastUsedCourse(course)
                }
            }
        }

        Controls.ScrollBar.vertical: ScrollBar { }
    }






}
