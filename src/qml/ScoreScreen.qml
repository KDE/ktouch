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
import org.kde.charts 0.1 as Charts

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
        courseFilter: screen.visible? screen.course: null
        lessonFilter: screen.visible && filterByLesson? screen.lesson: null
    }

    ErrorsModel {
        id: errorsModel
        trainingStats: screen.visible? screen.stats: null
    }

    Balloon {
        id: chartTypeDialog
        visualParent: chartTypeButton

        Column {
            id: chartTypeDialogContents

            PlasmaComponents.ToolButton {
                id: progressChartButton
                property Item tab: learningProgressTab
                width: Math.max(minimumWidth, errorsChartButton.minimumWidth)
                text: tab.title
                iconSource: tab.iconSource
                onClicked: {
                    tabGroup.currentTab = tab
                    chartTypeDialog.close()
                }
            }
            PlasmaComponents.ToolButton {
                id: errorsChartButton
                property Item tab: errorsTab
                width: Math.max(minimumWidth, progressChartButton.minimumWidth)
                text: tab.title
                iconSource: tab.iconSource
                onClicked: {
                    tabGroup.currentTab = tab
                    chartTypeDialog.close()
                }
            }
        }
    }

    Balloon {
        id: learningProgressDialog
        visualParent: learningProgressFilterButton

        Column {
            id: learningProgressDialogContents

            PlasmaComponents.ToolButton {
                id: allLessonsButton
                width: Math.max(implicitWidth, thisLessonButton.implicitWidth)
                text: i18n("All Lessons")
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
                text: i18n("This Lesson")
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

    Balloon {
        id: learningProgressPointTooltip
        visualParent: parent
        property int row: -1

        function findLessonTitle(id) {
            var course = screen.course
            for (var i = 0; i < course.lessonCount; i++) {
                if (course.lesson(i).id === id) {
                    return course.lesson(i).title
                }
            }
            return i18n("<i>Unknown</i>")
        }

        InformationTable {
            property list<InfoItem> infoModel: [
                InfoItem {
                    title: i18nc("Statistics on lesson:", "On:")
                    text: learningProgressPointTooltip.row !== -1? learningProgressPointTooltip.findLessonTitle(learningProgressModel.lessonId(learningProgressPointTooltip.row)): ""
                },
                InfoItem {
                    title: i18n("Accuracy:")
                    text: learningProgressPointTooltip.row !== -1? strFormatter.formatAccuracy(learningProgressModel.accuracy(learningProgressPointTooltip.row)): ""
                },
                InfoItem {
                    title: i18n("Characters per Minute:")
                    text: learningProgressPointTooltip.row !== -1? learningProgressModel.charactersPerMinute(learningProgressPointTooltip.row): ""
                }
            ]
            width: 250
            model: infoModel
        }
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

    SequentialAnimation {
        id: toggleLearningProgressFilterAnimation
        NumberAnimation
        {
            target: learningProgressChart
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
            target: learningProgressChart
            property: "opacity"
            to: 1
            duration: 250
            easing.type: Easing.InOutQuart
        }
    }

    Column {
        anchors.fill: parent

        spacing: header.margins.bottom / 2

        PlasmaComponents.ToolBar {
            id: header
            width: parent.width

            tools: Row {

                anchors.leftMargin: 10
                anchors.rightMargin: 10
                spacing: anchors.leftMargin

                PlasmaComponents.ToolButton {
                    id: homeScreenButton
                    text: i18n("Return to Home Screen")
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
                    text: i18n("Repeat Lesson")
                    onClicked: screen.lessonRepetionRequested()
                }

                PlasmaComponents.ToolButton {
                    id: nextLessonButton
                    iconSource: "go-next-view"
                    text: i18n("Next Lesson")
                    enabled: !!internal.nextLesson
                    onClicked: screen.nextLessonRequested(internal.nextLesson)
                }
            }
        }

        Rectangle {
            width: parent.width
            height: parent.height - ((parent.children.length - 1) * parent.spacing) - header.height
            color: theme.backgroundColor

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
                                    i18n("Congratulations! You have passed the lesson."):
                                    i18n("You have not passed the lesson.")
                                height: paintedHeight
                                font.pointSize: 1.5 * theme.defaultFont.pointSize
                                color: "#000000"
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
                            color: "#000000"
                        }
                    }
                }

                Rectangle {
                    id: statsBox
                    width: parent.width
                    height: 140
                    color: "#cccccc"
                    radius: 15

                    StatBox {
                        anchors.centerIn: parent
                        width: parent.width - 26
                        stats: screen.stats
                        referenceStats: screen.referenceStats
                    }
                }

                Item {
                    id: contentSpacer
                    height: 10
                    width: parent.width
                }

                Row {
                    id: chartControls
                    spacing: 5
                    width: parent.width

                    PlasmaComponents.Label {
                        id: showLabel
                        color: "#888"
                        anchors.verticalCenter: parent.verticalCenter
                        text: i18nc("Show a specific type of statistic", "Show")
                        height: paintedHeight
                    }

                    PlasmaComponents.ToolButton {
                        id: chartTypeButton
                        anchors.verticalCenter: parent.verticalCenter
                        text: tabGroup.currentTab.title
                        iconSource: tabGroup.currentTab.iconSource
                        width: chartTypeDialogContents.width
                        checked: chartTypeDialog.status === PlasmaComponents.DialogStatus.Open || chartTypeDialog.status === PlasmaComponents.DialogStatus.Opening
                        onClicked: {
                            if (checked) {
                                chartTypeDialog.close()
                            }
                            else {
                                chartTypeDialog.open()
                            }

                        }
                    }

                    PlasmaComponents.Label {
                        id: overLabel
                        color: "#888"
                        anchors.verticalCenter: parent.verticalCenter
                        text: i18nc("Show a statistic over one or more lessons", "Over")
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
                        width: parent.width - showLabel.width - chartTypeButton.width - overLabel.width - learningProgressFilterButton.width - accuracyLegend.width - charactersPerMinuteLegend.width - (parent.children.length - 1) * parent.spacing

                    }

                    Charts.LegendItem {
                        id: accuracyLegend
                        anchors.verticalCenter: parent.verticalCenter
                        dimension: learningProgressChart.accuracy
                        opacity: tabGroup.currentTab === learningProgressTab? 1: 0
                        Behavior on opacity {
                            NumberAnimation {duration: 150}
                        }
                    }
                    Charts.LegendItem {
                        id: charactersPerMinuteLegend
                        anchors.verticalCenter: parent.verticalCenter
                        dimension: learningProgressChart.charactersPerMinute
                        opacity: tabGroup.currentTab === learningProgressTab? 1: 0
                        Behavior on opacity {
                            NumberAnimation {duration: 150}
                        }
                    }
                }

                PlasmaComponents.TabGroup {
                    id: tabGroup
                    width: parent.width
                    height: content.height - caption.height - statsBox.height - contentSpacer.height - chartControls.height - (content.children.length - 1) * content.spacing

                    PlasmaComponents.Page {
                        id: learningProgressTab
                        property string title: i18n("Progress")
                        property string iconSource: "office-chart-area"

                        LearningProgressChart {
                            id: learningProgressChart
                            anchors.fill: parent
                            model: learningProgressModel

                            onElemEntered: {
                                learningProgressPointTooltip.visualParent = elem
                                learningProgressPointTooltip.row = row
                                learningProgressPointTooltip.open()
                            }

                            onElemExited: {
                                learningProgressPointTooltip.close()
                            }
                        }
                    }

                    PlasmaComponents.Page {
                        id: errorsTab

                        property string title: i18n("Errors")
                        property string iconSource: "office-chart-bar"

                        Charts.BarChart{
                            anchors.fill: parent
                            model: errorsModel
                            pitch: 60
                            textRole: 3 // Qt::ToolTipRole

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
