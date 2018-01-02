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

import QtQuick 2.9
import QtQuick.Layouts 1.3
import ktouch 1.0
import QtGraphicalEffects 1.0

import "../common"

FocusScope {
    id: root
    property Profile profile
    property DataIndexCourse dataIndexCourse
    property KeyboardLayout selectedKeyboardLayout
    property string activeKeyboardLayoutName
    property alias course: courseItem
    property Lesson selectedLesson: null
    signal lessonSelected(Course course, Lesson lesson)

    function update() {
        if (!course.isValid) return;
        if (!profile) return;
        course.updateLastUnlockedLessonIndex();
        selectLastLesson()
    }

    function selectLastLesson() {
        var lessonId = profileDataAccess.courseProgress(profile, course.id, ProfileDataAccess.LastSelectedLesson);
        if (lessonId !== "") {
            for (var index = 0; index < course.lessonCount; index++) {
                if (course.lesson(index).id === lessonId) {
                    root.selectedLesson = course.lesson(index)
                    content.currentIndex = index
                    break
                }
            }
        }
    }

    onProfileChanged: update()

    onDataIndexCourseChanged: {
        root.selectedLesson = null;
        course.update();
        root.update();
    }

    Course {
        id: courseItem
        property int lastUnlockedLessonIndex: -1
        property bool editable: courseItem.isValid && courseItem.id === "custom_lessons"
        function update() {
            if (root.dataIndexCourse === null) {
                return
            }
            if (dataIndexCourse.id == "custom_lessons") {
                profileDataAccess.loadCustomLessons(root.profile, dataIndexCourse.keyboardLayoutName, courseItem)
            }
            else {
                if (isValid && courseItem.id === dataIndexCourse.id) {
                    return
                }
                dataAccess.loadCourse(dataIndexCourse, courseItem)
            }
        }
        function updateLastUnlockedLessonIndex() {
            lastUnlockedLessonIndex = 0;
            if (course.kind == Course.LessonCollection || profile.skillLevel === Profile.Advanced) {
                lastUnlockedLessonIndex = course.lessonCount - 1;
                return
            }
            var lastUnlockedLessonId = profileDataAccess.courseProgress(profile, course.id, ProfileDataAccess.LastUnlockedLesson);
            if (lastUnlockedLessonId !== "") {
                for (var index = 0; index < course.lessonCount; index++) {
                    lastUnlockedLessonIndex = index
                    if (course.lesson(index).id === lastUnlockedLessonId) {
                        return
                    }
                }
            }
        }

        function createNewCustomLesson() {
            var lesson = ktouch.createLesson();
            lesson.id = utils.uuid()
            profileDataAccess.storeCustomLesson(lesson, root.profile, root.selectedKeyboardLayout.name)
            course.addLesson(lesson)
            updateLastUnlockedLessonIndex()
            content.currentIndex = course.indexOfLesson(lesson)
            root.selectedLesson = lesson
            lessonEditorDialog.open()
        }

        function deleteCustomLesson() {
            var msgItem = lessonDeletedMessageComponent.createObject(header);
            msgItem.deletedLesson.copyFrom(selectedLesson);
            profileDataAccess.deleteCustomLesson(selectedLesson.id);
            course.removeLesson(course.indexOfLesson(selectedLesson));
            root.selectedLesson = null;
            content.currentIndex = -1;
        }

        function restoreCustomLesson(lesson) {
            course.addLesson(lesson);
            profileDataAccess.storeCustomLesson(lesson, root.profile, root.selectedKeyboardLayout.name)
            content.currentIndex = course.indexOfLesson(lesson);
            updateLastUnlockedLessonIndex();
        }

        Component.onCompleted: update()
    }

    StatPopupDialog {
        id: statPopupDialog
        profile: root.profile
        course: course
        lesson: root.selectedLesson
    }

    LessonEditorDialog {
        id: lessonEditorDialog
        profile: root.profile
        keyboardLayout: root.selectedKeyboardLayout
        lesson: root.selectedLesson
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: header.height
            z: 2

            Column {
                id: header
                width: parent.width

                ToolBar {
                    id: toolbar
                    width: parent.width
                    dimFactor: 1.5

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 20
                        spacing: 5

                        Label {
                            text: root.course? root.course.title: ""
                            font.bold: true
                            color: toolbar.colorScheme.normalText
                        }

                        IconToolButton {
                            id: toggleCourseDesciptionButton
                            icon: "help-about"
                            checkable: true
                            color: toolbar.colorScheme.normalText
                            backgroundColor: toolbar.colorScheme.normalBackground
                            Layout.fillHeight: true
                            Layout.preferredWidth: toolbar.height
                        }

                        ToolSeparator {
                            visible: courseItem.editable
                        }

                        IconToolButton {
                            id: newLessonButton
                            icon: "document-new"
                            text: "Add New Lesson"
                            color: toolbar.colorScheme.normalText
                            backgroundColor: toolbar.colorScheme.normalBackground
                            visible: courseItem.editable
                            Layout.fillHeight: true
                            onClicked: {
                                course.createNewCustomLesson()
                            }
                        }

                        Item {
                            Layout.fillWidth: true
                        }

                        IconToolButton {
                            id: configureButton
                            icon: "application-menu"
                            color: toolbar.colorScheme.normalText
                            backgroundColor: toolbar.colorScheme.normalBackground
                            Layout.fillHeight: true
                            Layout.preferredWidth: toolbar.height
                            onClicked: {
                                var position = mapToItem(null, 0, height)
                                ktouch.showMenu(position.x, position.y)
                            }
                        }
                    }
                }

                CourseDescriptionItem {
                    id: courseDescriptionItem
                    width: parent.width
                    collapsed: !toggleCourseDesciptionButton.checked
                    description: courseItem.description
                }

                KeyboardLayoutMismatchMessage {
                    width: parent.width
                    collapsed: !root.course || !root.course.isValid || root.activeKeyboardLayoutName == root.course.keyboardLayoutName
                }

                Component {
                    id: lessonDeletedMessageComponent
                    LessonDeletedMessage {
                        width: parent.width
                        onUndeleteRequested: {
                            course.restoreCustomLesson(deletedLesson)
                        }
                    }
                }
            }

            DropShadow {
                anchors.fill: header
                source: header
                samples: 16
                horizontalOffset: 0
                verticalOffset: 0
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
            z: 1

            Rectangle {
                anchors.fill: parent
                color: content.colorScheme.shade(content.colorScheme.normalBackground, KColorScheme.DarkShade, 1, 0.0)
            }


            GridView {
                id: content
                anchors.fill: parent
                anchors.leftMargin: 20
                clip: true
                focus: true
                background.color: colorScheme.shade(colorScheme.normalBackground, KColorScheme.DarkShade, 1, 0.0)
                property int columns: Math.floor(width / (300 + 20))
                cellWidth: Math.floor(content.width / content.columns)
                cellHeight: Math.round(cellWidth * 2 / 3)

                Keys.onPressed: {
                    if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                        event.accepted = true;
                        if (root.selectedLesson && content.currentIndex <= course.lastUnlockedLessonIndex) {
                            lessonSelected(course, root.selectedLesson)
                        }
                    }
                }

                model: LessonModel {
                    id: lessonModel
                    course: courseItem
                }

                onCurrentIndexChanged: {
                    if (lessonModel.rowCount() > 0 && currentIndex != -1) {
                        root.selectedLesson = lessonModel.data(lessonModel.index(currentIndex, 0), LessonModel.DataRole)
                    }
                    else {
                        root.selectedLesson = null
                    }
                }

                delegate: Item {
                    id: item
                    width: content.cellWidth
                    height: content.cellHeight

                    LessonSelectorItem {
                        id: lessonItem
                        anchors.fill: parent
                        anchors.topMargin: 10
                        anchors.leftMargin: 0
                        anchors.rightMargin: 20
                        anchors.bottomMargin: 10
                        anchors.centerIn: parent
                        lesson: dataRole
                        selected:  content.currentIndex == index
                        editable: courseItem.editable
                        onClicked: {
                            item.forceActiveFocus()
                            content.currentIndex = index
                        }

                        onDoubleClicked: {
                            if (index <= course.lastUnlockedLessonIndex) {
                                lessonSelected(course, dataRole)
                            }
                        }
                        onStatButtonClicked: {
                            statPopupDialog.open()
                        }
                        onEditButtonClicked: {
                            lessonEditorDialog.open()
                        }
                        onDeleteButtonClicked: {
                            course.deleteCustomLesson();
                        }
                    }

                    LessonLockedNotice  {
                        anchors.centerIn: parent
                        visible: index > course.lastUnlockedLessonIndex
                        glowColor: lessonItem.background.color
                    }
                }
            }
        }

        Item {
            Layout.fillWidth: true
            height: footer.height
            z: 2

            ToolBar {
                id: footer
                width: parent.width

                KColorScheme {
                    id: footerColorScheme
                    colorGroup: KColorScheme.Active
                    colorSet: KColorScheme.Window
                }

                background: Rectangle {
                    color: footerColorScheme.normalBackground
                }

                RowLayout {
                    anchors.fill: parent
                    spacing: 0

                    Item {
                        Layout.fillWidth: true
                        height: startButton.implicitHeight

                        IconButton {
                            id: startButton
                            icon: "go-next-view"
                            bgColor: colorScheme.positiveBackground
                            anchors.centerIn: parent
                            text: i18n("Start Training")
                            enabled: root.selectedLesson && content.currentIndex <= course.lastUnlockedLessonIndex
                            onClicked: lessonSelected(course, root.selectedLesson)
                        }
                    }
                }
            }

            DropShadow {
                anchors.fill: footer
                source: footer
                samples: 16
                horizontalOffset: 0
                verticalOffset: 0
            }
        }
    }
}

