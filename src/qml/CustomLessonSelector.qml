/*
 *  Copyright 2013  Sebastian Gottfried <sebastiangottfried@web.de>
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
    property Profile profile
    property KeyboardLayout keyboardLayout
    property string keyboardLayoutName
    property alias course: courseItem
    signal lessonSelected(variant course, variant lesson)

    function update() {
        if (!profile)
            return
        profileDataAccess.loadCustomLessons(profile, keyboardLayoutName, courseItem)
    }

    QtObject {
        id: internal
        property variant deletedLessons: []
    }

    function selectLastLesson() {
        if (!course.isValid)
            return;
        var lessonId = profileDataAccess.courseProgress(profile, course.id, ProfileDataAccess.LastSelectedLesson);
        if (lessonId !== "") {
            for (var index = 0; index < course.lessonCount; index++) {
                if (course.lesson(index).id === lessonId) {
                    lessonList.currentIndex = index
                    break
                }
            }
        }
    }

    function createNewLesson() {
        tmpLesson.id = uuid()
        tmpLesson.title = ""
        tmpLesson.text = ""
        if (showCustomLessonDialog(tmpLesson, root.keyboardLayout)) {
            profileDataAccess.storeCustomLesson(tmpLesson, root.profile, keyboardLayoutName)
            update()
            lessonList.currentIndex = lessonList.count - 2
        }
    }

    function editLesson() {
        tmpLesson.copyFrom(base.selectedLesson)
        if (showCustomLessonDialog(tmpLesson, root.keyboardLayout)) {
            profileDataAccess.storeCustomLesson(tmpLesson, root.profile, keyboardLayoutName)
            update()
        }
    }

    function deleteLesson() {
        var deletedLessons = internal.deletedLessons
        var lesson = Qt.createQmlObject("import ktouch 1.0; Lesson{}", internal, "lesson")
        lesson.copyFrom(base.selectedLesson)
        deletedLessons.push(lesson)
        base.selectedLesson = null
        profileDataAccess.deleteCustomLesson(lesson.id)
        update()
        internal.deletedLessons = deletedLessons
    }

    function undoLessonDeletion() {
        var deletedLessons = internal.deletedLessons
        var lesson = deletedLessons.pop()
        internal.deletedLessons = deletedLessons
        profileDataAccess.storeCustomLesson(lesson, root.profile, keyboardLayoutName)
        update()
    }

    function confirmLessonDeletion() {
        var deletedLessons = internal.deletedLessons
        var lesson = deletedLessons.pop()
        internal.deletedLessons = deletedLessons
    }

    function updateSelectedLesson() {
        if (lessonList.currentIndex !== -1) {
            base.selectedLesson = lessonList.currentItem.lesson
        }
        else {
            base.selectedLesson = null
        }
    }

    onProfileChanged: update()
    onKeyboardLayoutNameChanged: update()

    Course {
        id: courseItem
    }

    Lesson {
        id: tmpLesson
    }

    LessonSelectorBase {
        id: base

        anchors.fill: parent

        list: ListView {
            id: lessonList
            anchors.fill: parent

            model: course.isValid? course.lessonCount + 1: 0

            clip: true

            delegate: ListItem {
                property Lesson lesson: index < course.lessonCount? course.lesson(index): null
                property bool isNewButton: index == course.lessonCount
                width: lessonList.width - scrollBar.width
                onClicked: {
                    lessonList.currentIndex = index
                    if (isNewButton) {
                        createNewLesson()
                    }
                }
                onDoubleClicked: {
                    if (!isNewButton) {
                        lessonSelected(course, lesson)
                    }
                }
                title: isNewButton? i18n("Create New Custom Lesson"): (lesson? lesson.title: "")
                iconSource: isNewButton? "list-add": ""
                label.font.italic: isNewButton
            }

            onCurrentIndexChanged: updateSelectedLesson()

            onModelChanged: selectLastLesson()

            PlasmaComponents.ScrollBar {
                id: scrollBar
                flickableItem: lessonList
            }
        }

        InlineToolbar {
            id: lessonToolbar
            parent: base.previewArea
            anchors {
                top: parent.top
                horizontalCenter: parent.horizontalCenter
                topMargin: 5
            }
            opacity: base.selectedLesson !== null? 1: 0
            content: [
                PlasmaComponents.ToolButton {
                    iconSource: "document-edit"
                    text: i18n("Edit")
                    onClicked: editLesson()
                    width: minimumWidth
                },
                PlasmaComponents.ToolButton {
                    iconSource: "edit-delete"
                    text: i18n("Delete")
                    width: minimumWidth
                    onClicked: deleteLesson()
                }
            ]
        }

        InlineToolbar {
            id: undoToolbar
            property Lesson lastDeletedLesson: internal.deletedLessons.length > 0? internal.deletedLessons[internal.deletedLessons.length - 1]: null
            property string lastDeletedLessonTitle: ""
            onLastDeletedLessonChanged: {
                if (!!lastDeletedLesson) {
                    lastDeletedLessonTitle = lastDeletedLesson.title
                }
            }

            parent: screen
            color: "#eee4be"
            anchors {
                top: parent.top
                horizontalCenter: parent.horizontalCenter
                topMargin: 15
            }
            opacity: !!lastDeletedLesson? 1: 0
            content: [
                PlasmaComponents.Label {
                    text: i18n("'%1' deleted.", undoToolbar.lastDeletedLessonTitle)
                },
                Item {
                    width: 5
                    height: 1
                },
                PlasmaComponents.ToolButton {
                    iconSource: "edit-undo"
                    text: i18n("Undo")
                    width: minimumWidth
                    onClicked: undoLessonDeletion()
                },
                PlasmaComponents.ToolButton {
                    iconSource: "dialog-close"
                    text: i18n("Dismiss")
                    width: minimumWidth
                    onClicked: confirmLessonDeletion()
                }
            ]
        }

        selectedLesson: null
        selectedLessonLocked: false
        onStartButtonClicked: lessonSelected(course, lessonList.currentItem.lesson)
    }
}
