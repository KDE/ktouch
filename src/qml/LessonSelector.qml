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
    id: item
    property Profile profile
    property string coursePath
    property alias course: courseItem
    signal lessonSelected(variant course, int lessonIndex)

    function update() {
        if (!course.isValid) return;
        if (!profile) return;
        selectLastLesson()
        enableUnlockedLessons()
    }

    function selectLastLesson() {
        var lessonId = profileDataAccess.courseProgress(profile, course.id, ProfileDataAccess.LastSelectedLesson);
        if (lessonId !== "") {
            for (var index = 0; index < course.lessonCount; index++) {
                if (course.lesson(index).id === lessonId) {
                    lessonList.currentIndex = index;
                    break;
                }
            }
        }
    }

    function enableUnlockedLessons() {
        if (profile.skillLevel === Profile.Advanced) {
            lessonList.lastUnlockedIndex = course.lessonCount - 1;
            return;
        }

        lessonList.lastUnlockedIndex = 0;
        var lessonId = profileDataAccess.courseProgress(profile, course.id, ProfileDataAccess.LastUnlockedLesson);
        if (lessonId !== "") {
            for (var index = 0; index < course.lessonCount; index++) {
                if (course.lesson(index).id === lessonId) {
                    lessonList.lastUnlockedIndex = index;
                    break;
                }
            }
        }
    }

    onProfileChanged: update()

    onCoursePathChanged: {
        course.update()
        update()
    }

    Course {
        id: courseItem
        property string path
        function update() {
            if (isValid && path === coursePath)
                return
            path = coursePath
            dataAccess.loadCourse(path, course)
        }
        Component.onCompleted: update()
    }

    Row {
        anchors.fill: parent
        anchors.margins: 5
        spacing: 20

        Item {
            id: listContainer
            height: parent.height
            width: Math.round((parent.width - parent.spacing) / 2)

            ListView {
                id: lessonList
                property int lastUnlockedIndex: 0
                anchors.fill: parent
                model: course.isValid? course.lessonCount: 0
                clip: true
                delegate: ListItem {
                    property Lesson lesson: index < course.lessonCount? course.lesson(index): null
                    property bool locked: index > lessonList.lastUnlockedIndex
                    width: lessonList.width - scrollBar.width
                    onSelected: lessonList.currentIndex = index
                    iconSource: locked? "object-locked": ""
                    label.opacity: locked? 0.5: 1.0
                    title: lesson? lesson.title: ""
                }
                onModelChanged: update()
            }

            PlasmaComponents.ScrollBar {
                id: scrollBar
                flickableItem: lessonList
            }
        }

        Column {
            width: parent.width - listContainer.width - parent.spacing
            height: parent.height

            LessonPreview {
                width: parent.width
                height: parent.height - startButtonContainer.height
                lesson: lessonList.currentItem != null? lessonList.currentItem.lesson: null

                LessonLockedNotice {
                    anchors.centerIn: parent
                    opacity: lessonList.currentItem !== null && lessonList.currentItem.locked? 1: 0
                }
            }

            Item {
                id: startButtonContainer
                width: parent.width
                height: Math.round(1.5 * startButton.height)

                PlasmaComponents.Button {
                    id: startButton
                    anchors {
                        horizontalCenter: startButtonContainer.horizontalCenter
                        bottom: parent.bottom
                    }
                    text: i18n("Start training")
                    enabled: lessonList.currentItem !== null && !lessonList.currentItem.locked
                    iconSource: "go-next-view"
                    onClicked: {
                        var lesson = course.lesson(lessonList.currentIndex)
                        profileDataAccess.saveCourseProgress(lesson.id, profile, course.id, ProfileDataAccess.LastSelectedLesson)
                        lessonSelected(course, lessonList.currentIndex)
                    }
                }
            }
        }
    }
}
