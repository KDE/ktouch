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
import ktouch 1.0
import org.kde.qtextracomponents 0.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

FocusScope {
    id: screen

    function start() {}
    function reset() {
        internal.lessonPassed = 100 * stats.accuracy >= preferences.requiredAccuracy && stats.charactersPerMinute >= preferences.requiredStrokesPerMinute

        var lessonIndex = 0;
        for (var i = 0; i < course.lessonCount; i++) {
            if (lesson === course.lesson(i)) {
                lessonIndex = i
                break
            }
        }

        var lastUnlockedLessonIndex = 0
        if (profile.skillLevel === Profile.Advanced) {
            lastUnlockedLessonIndex = course.lessonCount - 1;
            internal.nextLessonUnlocked = false
        }
        else {
            var lastUnlockedLessonId = profileDataAccess.courseProgress(profile, course.id, ProfileDataAccess.LastUnlockedLesson);
            if (lastUnlockedLessonId !== "") {
                for (var index = 0; index < course.lessonCount; index++) {
                    if (course.lesson(index).id === lastUnlockedLessonId) {
                        lastUnlockedLessonIndex = index;
                        break;
                    }
                }
            }

            internal.nextLessonUnlocked = internal.lessonPassed && lessonIndex === lastUnlockedLessonIndex && lessonIndex + 1 < course.lessonCount

            if (internal.nextLessonUnlocked) {
                lastUnlockedLessonIndex++
            }
        }

        if (lessonIndex + 1 < course.lessonCount && lessonIndex + 1 <= lastUnlockedLessonIndex) {
            internal.nextLesson = course.lesson(lessonIndex + 1)
        }

        if (internal.nextLessonUnlocked) {
            profileDataAccess.saveCourseProgress(internal.nextLesson.id, profile, course.id, ProfileDataAccess.LastUnlockedLesson)
        }
    }

    property Profile profile
    property Lesson lesson
    property Course course
    property TrainingStats stats
    property TrainingStats referenceStats

    signal homeScreenRequested
    signal nextLessonRequested(variant lesson)
    signal lessonRepetionRequested

    QtObject {
        id: internal
        property bool lessonPassed: false
        property bool nextLessonUnlocked: false
        property Lesson nextLesson: null
    }

    Column {
        anchors.fill: parent
        spacing: 15

        PlasmaComponents.ToolBar {
            id: header
            width: parent.width

            tools: Row {

                anchors.leftMargin: 10
                anchors.rightMargin: 10
                spacing: anchors.leftMargin


                PlasmaComponents.Label {
                    id: headerLabel
                    anchors {
                        verticalCenter: parent.verticalCenter
                    }
                    text: i18n("Lesson finished")
                    height: paintedHeight
                    font.pointSize: 1.5 * theme.defaultFont.pointSize
                }

                PlasmaComponents.TabBar {
                    id: tabBar
                    width: parent.width - headerLabel.width - parent.spacing

                    PlasmaComponents.TabButton {
                        tab: summaryTab
                        text: i18n("Summary")
                    }
                    PlasmaComponents.TabButton {
                        tab: speedTab
                        text: i18n("Speed")
                    }
                    PlasmaComponents.TabButton {
                        tab: accuracyTab
                        text: i18n("Accuracy")
                    }
                }
            }
        }

        PlasmaComponents.TabGroup {
            id: tabGroup
            width: parent.width - 20
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height - ((parent.children.length - 1) * parent.spacing) - header.height -  footer.height

            PlasmaComponents.Page {
                id: summaryTab
                Column {
                    anchors.fill: parent
                    spacing: 15

                    Row {
                        width: childrenRect.width
                        height: childrenRect.height
                        spacing: 10
                        EllapsedTimeMeter {
                            ellapsedTime: stats.ellapsedTime
                            referenceEllapsedTime: referenceStats.isValid? referenceStats.ellapsedTime: stats.ellapsedTime
                        }
                        CharactersPerMinuteMeter {
                            charactersPerMinute: stats.charactersPerMinute
                            referenceCharactersPerMinute: referenceStats.isValid? referenceStats.charactersPerMinute: stats.charactersPerMinute
                        }
                        AccuracyMeter {
                            accuracy: stats.accuracy
                            referenceAccuracy: referenceStats.isValid? referenceStats.accuracy: stats.accuracy
                        }
                    }

                    Rectangle {
                        color: "#fff"
                        radius: 5
                        border {
                            color: "#000"
                            width: 1
                        }

                        width: childrenRect.width + 10
                        height: childrenRect.height + 10

                        Row {
                            anchors.centerIn: parent
                            spacing: 5
                            QIconItem {
                                icon: QIcon(internal.lessonPassed? "dialog-ok-apply": "dialog-cancel")
                                width: theme.smallIconSize
                                height: theme.smallIconSize
                            }
                            Text {
                                anchors.verticalCenter: parent.verticalCenter
                                text: {
                                    if (internal.lessonPassed && internal.nextLessonUnlocked)
                                        return i18n("Congratulations! You've passed the lesson und you have unlocked the next lesson.")
                                    if (internal.lessonPassed)
                                        return i18n("Congratulations! You've passed the lesson.")
                                    return i18n("You haven't passed the lesson and you should repeat it to improve your skills.")
                                }
                            }
                        }

                    }

                }
            }

            PlasmaComponents.Page {
                id: speedTab
                Item {
                    anchors.fill: parent
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 50
                        color: "#888"
                        text: "TODO"
                    }
                }
            }

            PlasmaComponents.Page {
                id: accuracyTab
                Item {
                    anchors.fill: parent
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 50
                        color: "#888"
                        text: "TODO"
                    }
                }
            }
        }

        Row {
            id: footer
            width: parent.width - 20
            anchors.horizontalCenter: parent.horizontalCenter
            height: childrenRect.height + 5
            spacing: 5

            PlasmaComponents.Button {
                id: homeScreenButton
                text: i18n("Return to home screen")
                iconSource: "go-home"
                onClicked: screen.homeScreenRequested()
            }
            Item {
                width: parent.width - ((parent.children.length - 1) * parent.spacing) - nextLessonButton.width - repeatLessonButton.width - homeScreenButton.width
                height: homeScreenButton.height
            }
            PlasmaComponents.Button {
                id: repeatLessonButton
                iconSource: "view-refresh"
                text: i18n("Repeat lesson")
                onClicked: screen.lessonRepetionRequested()
            }
            PlasmaComponents.Button {
                id: nextLessonButton
                iconSource: "go-next-view"
                text: i18n("Next lesson")
                enabled: !!internal.nextLesson
                onClicked: screen.nextLessonRequested(internal.nextLesson)
            }
        }
    }
}
