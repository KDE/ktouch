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
    property variant course
    signal lessonSelected(variant lesson)

    function selectLastLesson() {
        if (!course) return;
        if (!profile) return;
        var lessonId = profileDataAccess.courseProgress(profile, course.id, ProfileDataAccess.LastSelectedLesson);
        if (lessonId !== "") {
            for (var index = 0; index < course.lessonCount; index++) {
                if (course.lesson(index).id === lessonId) {
                    list.currentIndex = index;
                    break;
                }
            }
        }
    }

    onProfileChanged: selectLastLesson()
    onCourseChanged: selectLastLesson()

    Row {
        anchors.fill: parent
        anchors.margins: 5
        spacing: 20

        Item {
            id: listContainer
            height: parent.height
            width: Math.round((parent.width - parent.spacing) / 2)

            ListView {
                id: list
                anchors .fill: parent
                model: course.lessonCount
                clip: true
                delegate: ListItem {
                    width: list.width - scrollBar.width
                    onSelected: list.currentIndex = index
                    title: item.course.lesson(index).title
                }
                onModelChanged: selectLastLesson()
            }

            PlasmaComponents.ScrollBar {
                id: scrollBar
                flickableItem: list
            }
        }

        Column {
            width: parent.width - listContainer.width - parent.spacing
            height: parent.height

            LessonPreview {
                width: parent.width
                height: parent.height - startButtonContainer.height
                lesson: list.currentIndex !== -1? item.course.lesson(list.currentIndex): null
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
                    enabled: list.currentItem !== null
                    iconSource: "go-next-view"
                    onClicked: {
                        var lesson = item.course.lesson(list.currentIndex)
                        profileDataAccess.saveCourseProgress(lesson.id, profile, course.id, ProfileDataAccess.LastSelectedLesson)
                        lessonSelected(lesson)
                    }
                }
            }
        }
    }
}
