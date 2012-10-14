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
import org.kde.ktouch.graph 0.1 as Graph

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

        internal.nextLesson = null;

        if (lessonIndex + 1 < course.lessonCount && lessonIndex + 1 <= lastUnlockedLessonIndex) {
            internal.nextLesson = course.lesson(lessonIndex + 1)
        }

        if (internal.nextLessonUnlocked) {
            profileDataAccess.saveCourseProgress(internal.nextLesson.id, profile, course.id, ProfileDataAccess.LastUnlockedLesson)
        }

        learningProgressModel.update()
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

    LearningProgressModel {
        property bool filterByLesson: false
        id: learningProgressModel
        profile: screen.visible? screen.profile: null
        courseFilter: screen.course
        lessonFilter: filterByLesson? screen.lesson: null
    }

    ErrorsModel {
        id: errorsModel
        trainingStats: screen.visible? screen.stats: null
    }

    Baloon {
        id: graphTypeDialog
        visualParent: graphTypeButton

        Column {
            id: graphTypeDialogContents

            PlasmaComponents.ToolButton {
                id: progressGraphButton
                property Item tab: learningProgressTab
                width: Math.max(implicitWidth, errorsGraphButton.implicitWidth)
                text: tab.title
                iconSource: tab.iconSource
                onClicked: {
                    tabGroup.currentTab = tab
                    graphTypeDialog.close()
                }
            }
            PlasmaComponents.ToolButton {
                id: errorsGraphButton
                property Item tab: errorsTab
                width: Math.max(implicitWidth, progressGraphButton.implicitWidth)
                text: tab.title
                iconSource: tab.iconSource
                onClicked: {
                    tabGroup.currentTab = tab
                    graphTypeDialog.close()
                }
            }
        }
    }

    Baloon {
        id: learningProgressDialog
        visualParent: learningProgressFilterButton

        Column {
            id: learningProgressDialogContents

            PlasmaComponents.ToolButton {
                id: allLessonsButton
                width: Math.max(implicitWidth, thisLessonButton.implicitWidth)
                text: i18n("All lessons")
                iconSource: "view-filter"
                onClicked: {
                    if (learningProgressModel.filterByLesson) {
                        toggleLearningProgressFilterAnimation.restart()
                    }
                    learningProgressDialog.close()
                }
            }
            PlasmaComponents.ToolButton {
                id: thisLessonButton
                width: Math.max(implicitWidth, allLessonsButton.implicitWidth)
                text: i18n("This lesson")
                iconSource: "view-filter"
                onClicked: {
                    if (!learningProgressModel.filterByLesson) {
                        toggleLearningProgressFilterAnimation.restart()
                    }
                    learningProgressDialog.close()
                }
            }
        }
    }

    SequentialAnimation {
        id: toggleLearningProgressFilterAnimation
        NumberAnimation
        {
            target: learningProgressGraph
            property: "opacity"
            to: 0
            duration: 250
            easing.type: Easing.InOutQuart
        }
        PropertyAction
        {
            target: learningProgressModel
            property: "filterByLesson"
            value: !learningProgressModel.filterByLesson
        }
        NumberAnimation
        {
            target: learningProgressGraph
            property: "opacity"
            to: 1
            duration: 250
            easing.type: Easing.InOutQuart
        }
    }

    Column {
        anchors.fill: parent

        PlasmaComponents.ToolBar {
            id: header
            width: parent.width

            tools: Row {

                anchors.leftMargin: 10
                anchors.rightMargin: 10
                spacing: anchors.leftMargin

                PlasmaComponents.ToolButton {
                    id: homeScreenButton
                    text: i18n("Return to home screen")
                    iconSource: "go-home"
                    onClicked: screen.homeScreenRequested()
                }

                Item {
                    height: parent.height
                    width: parent.width - homeScreenButton.width - repeatLessonButton.width - nextLessonButton.width - (parent.children.length - 1) * parent.spacing
                }

                PlasmaComponents.ToolButton {
                    id: repeatLessonButton
                    iconSource: "view-refresh"
                    text: i18n("Repeat lesson")
                    onClicked: screen.lessonRepetionRequested()
                }

                PlasmaComponents.ToolButton {
                    id: nextLessonButton
                    iconSource: "go-next-view"
                    text: i18n("Next lesson")
                    enabled: !!internal.nextLesson
                    onClicked: screen.nextLessonRequested(internal.nextLesson)
                }
            }
        }


        Rectangle {
            width: parent.width
            height: parent.height - ((parent.children.length - 1) * parent.spacing) - header.height
            color: "#fff"

            Column {
                id: content
                anchors.fill: parent
                anchors.margins: 20
                spacing: 15

                Rectangle {
                    id: caption
                    width: parent.width
                    color: "#eee4be"
                    height: captionContent.height + 30
                    radius: 15

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
                            QIconItem {
                                id: captionIcon
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                icon: QIcon(internal.lessonPassed? "dialog-ok-apply": "dialog-cancel")
                                width: height
                                height: captionLabel.height
                            }
                            PlasmaComponents.Label {
                                id: captionLabel
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                text:  internal.lessonPassed?
                                    i18n("Congratulations! You've passed the lesson."):
                                    i18n("You haven't passed the lesson.")
                                height: paintedHeight
                                font.pointSize: 1.5 * theme.defaultFont.pointSize
                            }
                        }
                        PlasmaComponents.Label {
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
                            height: paintedHeight
                        }
                    }
                }

                Rectangle {
                    id: statsBox
                    width: parent.width
                    height: 130
                    color: "#cccccc"
                    radius: 15
                    ElapsedTimeMeter {
                        anchors {
                            left: parent.left
                            verticalCenter: parent.verticalCenter
                            leftMargin: 15
                        }

                        elapsedTime: stats.elapsedTime
                        referenceElapsedTime: referenceStats.isValid? referenceStats.elapsedTime: stats.elapsedTime
                    }
                    CharactersPerMinuteMeter {
                        anchors {
                            horizontalCenter: parent.horizontalCenter
                            verticalCenter: parent.verticalCenter
                        }

                        charactersPerMinute: stats.charactersPerMinute
                        referenceCharactersPerMinute: referenceStats.isValid? referenceStats.charactersPerMinute: stats.charactersPerMinute
                    }
                    AccuracyMeter {
                        anchors {
                            right: parent.right
                            verticalCenter: parent.verticalCenter
                            rightMargin: 15
                        }
                        accuracy: stats.accuracy
                        referenceAccuracy: referenceStats.isValid? referenceStats.accuracy: stats.accuracy
                    }
                }

                Item {
                    id: contentSpacer
                    height: 10
                    width: parent.width
                }

                Row {
                    id: graphControls
                    spacing: 5
                    width: parent.width

                    PlasmaComponents.Label {
                        id: showLabel
                        color: "#888"
                        anchors.verticalCenter: parent.verticalCenter
                        text: i18n("Show")
                        height: paintedHeight
                    }

                    PlasmaComponents.ToolButton {
                        id: graphTypeButton
                        anchors.verticalCenter: parent.verticalCenter
                        text: tabGroup.currentTab.title
                        iconSource: tabGroup.currentTab.iconSource
                        width: graphTypeDialogContents.width
                        checked: graphTypeDialog.status === PlasmaComponents.DialogStatus.Open || graphTypeDialog.status === PlasmaComponents.DialogStatus.Opening
                        onClicked: {
                            if (checked) {
                                graphTypeDialog.close()
                            }
                            else {
                                graphTypeDialog.open()
                            }

                        }
                    }

                    PlasmaComponents.Label {
                        id: overLabel
                        color: "#888"
                        anchors.verticalCenter: parent.verticalCenter
                        text: i18n("Over")
                        height: paintedHeight
                        opacity: tabGroup.currentTab === learningProgressTab? 1: 0
                        Behavior on opacity {
                            NumberAnimation {duration: 150}
                        }
                    }

                    PlasmaComponents.ToolButton {
                        id: learningProgressFilterButton
                        anchors.verticalCenter: parent.verticalCenter
                        text: learningProgressModel.filterByLesson? thisLessonButton.text: allLessonsButton.text
                        iconSource: "view-filter"
                        checked: learningProgressDialog.status === PlasmaComponents.DialogStatus.Open || learningProgressDialog.status === PlasmaComponents.DialogStatus.Opening
                        opacity: tabGroup.currentTab === learningProgressTab? 1: 0
                        onClicked: {
                            if (checked) {
                                learningProgressDialog.close()
                            }
                            else {
                                learningProgressDialog.open()
                            }
                        }
                        Behavior on opacity {
                            NumberAnimation {duration: 150}
                        }
                    }

                    Item {
                        height: parent.height
                        width: parent.width - showLabel.width - graphTypeButton.width - overLabel.width - learningProgressFilterButton.width - accuracyLegend.width - charactersPerMinuteLegend.width - (parent.children.length - 1) * parent.spacing

                    }

                    Graph.LegendItem {
                        id: accuracyLegend
                        anchors.verticalCenter: parent.verticalCenter
                        dimension: learningProgressGraph.accuracy
                        opacity: tabGroup.currentTab === learningProgressTab? 1: 0
                        Behavior on opacity {
                            NumberAnimation {duration: 150}
                        }
                    }
                    Graph.LegendItem {
                        id: charactersPerMinuteLegend
                        anchors.verticalCenter: parent.verticalCenter
                        dimension: learningProgressGraph.charactersPerMinute
                        opacity: tabGroup.currentTab === learningProgressTab? 1: 0
                        Behavior on opacity {
                            NumberAnimation {duration: 150}
                        }
                    }
                }

                PlasmaComponents.TabGroup {
                    id: tabGroup
                    width: parent.width
                    height: content.height - caption.height - statsBox.height - contentSpacer.height - graphControls.height - (content.children.length - 1) * content.spacing

                    PlasmaComponents.Page {
                        id: learningProgressTab
                        property string title: i18n("Progress")
                        property string iconSource: "office-chart-area"

                        LearningProgressGraph {
                            id: learningProgressGraph
                            anchors.fill: parent
                            model: learningProgressModel
                        }
                    }

                    PlasmaComponents.Page {
                        id: errorsTab

                        property string title: i18n("Errors")
                        property string iconSource: "office-chart-bar"

                        Graph.BarGraph {
                            anchors.fill: parent
                            model: errorsModel
                            pitch: 60
                            textRole: 3 // Qt::ToolTipRole

                            dimensions: [
                                Graph.Dimension {
                                    dataColumn: 0
                                    color: "#ffb12d"
                                    maximumValue: Math.max(4, Math.ceil(errorsModel.maximumErrorCount / 4) * 4)
                                    label: i18n("Errors")
                                }
                            ]
                        }
                    }
                }
            }
        }
    }
}
