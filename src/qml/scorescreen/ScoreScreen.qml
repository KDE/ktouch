/*
 *  Copyright 2018  Sebastian Gottfried <sebastian.gottfried@posteo.de>
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
import org.kde.charts 0.1 as Charts

import "../common"
import "../meters"

FocusScope {
    id: screen

    function start() {}
    function reset() {
        internal.lessonPassed = Math.round(1000 * stats.accuracy) >= Math.round(10 * preferences.requiredAccuracy) && stats.charactersPerMinute >= preferences.requiredStrokesPerMinute

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

        internal.nextLesson = null;

        if (lessonIndex + 1 < course.lessonCount && lessonIndex + 1 <= lastUnlockedLessonIndex) {
            internal.nextLesson = course.lesson(lessonIndex + 1)
        }

        if (internal.nextLessonUnlocked) {
            profileDataAccess.saveCourseProgress(internal.nextLesson.id, profile, course.id, ProfileDataAccess.LastUnlockedLesson)
        }

        learningProgressModel.update()
    }

    function forceActiveFocus() {
        if (internal.lessonPassed && internal.nextLesson) {
            nextLessonButton.forceActiveFocus()
        }
        else {
            repeatLessonButton.forceActiveFocus()
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

    KColorScheme {
        id: colorScheme
        colorGroup: KColorScheme.Active
        colorSet: KColorScheme.View
    }



    LearningProgressModel {
        property bool filterByLesson: learningProgressFilterComboBox.currentIndex == 1
        id: learningProgressModel
        profile: screen.visible? screen.profile: null
        courseFilter: screen.visible? screen.course: null
        lessonFilter: screen.visible && filterByLesson? screen.lesson: null
    }

    ErrorsModel {
        id: errorsModel
        trainingStats: screen.visible? screen.stats: null
    }

    Balloon {
        id: errorsTooltip
        visualParent: parent
        property int row: -1

        InformationTable {
            property list<InfoItem> infoModel: [
                InfoItem {
                    title: i18n("Character:")
                    text: errorsTooltip.row !== -1? errorsModel.character(errorsTooltip.row): ""
                },
                InfoItem {
                    title: i18n("Errors:")
                    text: errorsTooltip.row !== -1? errorsModel.errors(errorsTooltip.row): ""
                }
            ]
            width: 250
            model: infoModel
        }
    }

    ColumnLayout {
        anchors.fill: parent

        spacing: 0

        ToolBar {
            id: header
            Layout.fillWidth: true

            RowLayout {

                anchors.fill: parent
                anchors.leftMargin: 20
                anchors.rightMargin: 20
                spacing: anchors.leftMargin

                IconToolButton {
                    id: homeScreenButton
                    text: i18n("Return to Home Screen")
                    iconName: "go-home"
                    colorScheme.colorSet: KColorScheme.Complementary
                    onClicked: screen.homeScreenRequested()
                }

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                IconToolButton {
                    id: repeatLessonButton
                    iconName: "view-refresh"
                    text: i18n("Repeat Lesson")
                    colorScheme.colorSet: KColorScheme.Complementary
                    onClicked: screen.lessonRepetionRequested()
                }

                IconToolButton {
                    id: nextLessonButton
                    iconName: "go-next-view"
                    text: i18n("Next Lesson")
                    enabled: internal.nextLesson
                    colorScheme.colorSet: KColorScheme.Complementary
                    onClicked: screen.nextLessonRequested(internal.nextLesson)
                }
            }
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: colorScheme.normalBackground

            ColumnLayout {
                id: content
                anchors.fill: parent
                anchors.margins: 20
                spacing: 15

                Rectangle {
                    id: caption
                    Layout.fillWidth: true
                    Layout.preferredHeight: captionContent.height + 30
                    radius: 15
                    color: "#eee4be"

                    Item {
                        id: captionContent
                        anchors.centerIn: parent
                        width: Math.max(mainCaption.width, subCaption.width)
                        height: subCaption.visible? mainCaption.height + subCaption.height + 5: mainCaption.height
                        Item {
                            id: mainCaption
                            anchors.top: parent.top
                            anchors.horizontalCenter: parent.horizontalCenter
                            width: captionIcon.width + captionLabel.width + 7
                            height: Math.max(captionIcon.height, captionLabel.height)
                            Icon {
                                id: captionIcon
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                icon: internal.lessonPassed? "dialog-ok-apply": "dialog-cancel"
                                width: height
                                height: captionLabel.height
                            }
                            Label {
                                id: captionLabel
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                text:  internal.lessonPassed?
                                           i18n("Congratulations! You have passed the lesson."):
                                           i18n("You have not passed the lesson.")
                                font.pointSize: 1.5 * Qt.font({'family': 'sansserif'}).pointSize
                                color: "#000000"
                            }
                        }
                        Label {
                            id: subCaption
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            visible: text !== ""
                            text: {
                                if (!internal.lessonPassed)
                                    return i18n("Repeat the lesson. Your skills will improve automatically over time.")
                                if (internal.nextLessonUnlocked)
                                    return i18n("Next lesson unlocked")
                                return ""
                            }
                            color: "#000000"
                        }
                    }
                }

                Rectangle {
                    id: statsBox
                    Layout.fillWidth: true
                    Layout.preferredHeight: 140
                    radius: 15
                    color: "#cccccc"

                    StatBox {
                        anchors.centerIn: parent
                        width: parent.width - 26
                        stats: screen.stats
                        referenceStats: screen.referenceStats
                    }
                }

                Item {
                    id: contentSpacer
                    Layout.preferredHeight: 10
                    Layout.fillWidth: true
                }

                RowLayout {
                    id: chartControls
                    spacing: 5
                    Layout.fillWidth: true

                    Label {
                        id: showLabel
                        Layout.alignment: Qt.AlignVCenter
                        text: i18nc("Show a specific type of statistic", "Show")
                        opacity: 0.7
                    }

                    ComboBox {
                        id: chartTypeComboBox
                        Layout.alignment: Qt.AlignVCenter
                        model: ListModel {
                            Component.onCompleted: {
                                append({"text": i18n("Progress"), "icon": "office-chart-area"});
                                append({"text": i18n("Errors"), "icon": "office-chart-bar"});
                            }
                        }
                        textRole: "text"
                        currentIndex: 0
                    }

                    Label {
                        id: overLabel
                        Layout.alignment: Qt.AlignVCenter
                        text: i18nc("Show a statistic over one or more lessons", "Over")
                        opacity: tabGroup.currentItem === learningProgressTab? 0.7: 0
                        Behavior on opacity {
                            NumberAnimation {duration: 150}
                        }
                    }

                    ComboBox {
                        id: learningProgressFilterComboBox
                        Layout.alignment: Qt.AlignVCenter
                        model: ListModel {
                            Component.onCompleted: {
                                append({"text": i18n("All Lessons"), "icon": "view-filter"});
                                append({"text": i18n("This Lessons"), "icon": "view-filter"});
                            }
                        }
                        textRole: "text"
                        currentIndex: 0
                        opacity: tabGroup.currentItem === learningProgressTab? 1: 0
                        Behavior on opacity {
                            NumberAnimation {duration: 150}
                        }
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    Charts.LegendItem {
                        id: accuracyLegend
                        textColor: colorScheme.normalText
                        Layout.alignment: Qt.AlignVCenter
                        opacity:  tabGroup.currentItem === learningProgressTab? 1: 0
                        Behavior on opacity {
                            NumberAnimation {duration: 150}
                        }
                    }
                    Charts.LegendItem {
                        id: charactersPerMinuteLegend
                        textColor: colorScheme.normalText
                        Layout.alignment: Qt.AlignVCenter
                        opacity:  tabGroup.currentItem === learningProgressTab? 1: 0
                        Behavior on opacity {
                            NumberAnimation {duration: 150}
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true


                    StackLayout {
                        anchors.fill: parent
                        id: tabGroup
                        currentIndex: chartTypeComboBox.currentIndex
                        property Item currentItem: currentIndex != -1? children[currentIndex]: null


                        LearningProgressChart {
                            id: learningProgressTab
                            property string title: i18n("Progress")
                            property string iconName: "office-chart-area"
                            model: learningProgressModel
                            backgroundColor: colorScheme.normalBackground

                            Component.onCompleted: {
                                accuracyLegend.dimension = learningProgressTab.accuracy
                                charactersPerMinuteLegend.dimension = learningProgressTab.charactersPerMinute
                            }
                            Component.onDestruction: {
                                accuracyLegend.dimension = null
                                charactersPerMinuteLegend.dimension = null
                            }
                        }

                        Item {
                            id: errorsTab
                            property string title: i18n("Errors")
                            property string iconName: "office-chart-bar"

                            Charts.BarChart{
                                model: errorsModel
                                pitch: 60
                                textRole: 3 // Qt::ToolTipRole
                                backgroundColor: colorScheme.normalBackground

                                dimensions: [
                                    Charts.Dimension {
                                        dataColumn: 0
                                        color: "#ffb12d"
                                        maximumValue: Math.max(4, Math.ceil(errorsModel.maximumErrorCount / 4) * 4)
                                        label: i18n("Errors")
                                    }
                                ]

                                onElemEntered: {
                                    errorsTooltip.visualParent = elem;
                                    errorsTooltip.row = row
                                    errorsTooltip.open()
                                }

                                onElemExited: {
                                    errorsTooltip.close()
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
