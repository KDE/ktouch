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
    signal lessonSelected(variant course, variant lesson)

    function update() {
        if (!profile) return;
        // selectLastLesson()
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

    onProfileChanged: update()

    CustomLessonModel {
        id: customLessonModel
        profile: root.profile
    }

    Course {
        id: dummyCourse
        Component.onCompleted: {
            dummyCourse.id = "custom_lessons"
        }
    }

    Lesson {
        id: lesson
    }

    LessonSelectorBase {
        id: base

        anchors.fill: parent

        list: ListView {
            id: lessonList
            anchors.fill: parent

            model: ColumnProxyModel {
                sourceModel: customLessonModel
                column: 1
            }

            clip: true

            delegate: ListItem {
                width: lessonList.width - scrollBar.width
                onClicked: lessonList.currentIndex = index
                onDoubleClicked: {
                    lessonSelected(dummyCourse, lesson)
                }
                title: display
            }

            onCurrentIndexChanged: {
                base.selectedLesson = null
                if (currentIndex !== -1) {
                    customLessonModel.loadLesson(currentIndex, lesson)
                    base.selectedLesson = lesson
                }
            }

            PlasmaComponents.ScrollBar {
                id: scrollBar
                flickableItem: lessonList
            }
        }

        selectedLesson: null
        selectedLessonLocked: false
        onStartButtonClicked: lessonSelected(dummyCourse, lesson)
    }
}
