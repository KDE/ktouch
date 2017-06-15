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

import "../common"

Item {
    id: item
    property Profile profile
    property DataIndexCourse dataIndexCourse
    property alias course: courseItem
    signal lessonSelected(variant course, variant lesson)

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

    onDataIndexCourseChanged: {
        course.update()
        update()
    }

    Course {
        id: courseItem
        function update() {
            if (item.dataIndexCourse === null)
                return
            if (isValid && courseItem.id === dataIndexCourse.id)
                return
            dataAccess.loadCourse(dataIndexCourse, courseItem)
        }
        Component.onCompleted: update()
    }

    LessonSelectorBase {
        anchors.fill: parent

        list:ScrollView {
            anchors.fill: parent
            ListView {
                anchors.fill: parent
                id: lessonList
                property int lastUnlockedIndex: 0
                model: course.isValid? course.lessonCount: 0
                spacing: 3
                clip: true
                delegate: ListItem {
                    property Lesson lesson: index < course.lessonCount? course.lesson(index): null
                    property bool locked: index > lessonList.lastUnlockedIndex
                    width: lessonList.width
                    onClicked: lessonList.currentIndex = index
                    onDoubleClicked: {
                        if (!locked) {
                            lessonSelected(course, lesson)
                        }
                    }
                    iconSource: locked? "object-locked": ""
                    label.opacity: locked? 0.5: 1.0
                    title: lesson? lesson.title: ""

                }
                onModelChanged: update()
            }
        }

        selectedLesson: lessonList.currentItem != null? lessonList.currentItem.lesson: null
        selectedLessonLocked: lessonList.currentItem !== null && lessonList.currentItem.locked
        onStartButtonClicked: lessonSelected(course, lessonList.currentItem.lesson)
    }
}
