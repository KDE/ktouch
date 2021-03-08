/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *  SPDX-FileCopyrightText: 2015 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import ktouch 1.0

import "./common"
import "./meters"
import "./homescreen"
import "./trainingscreen"
import "./scorescreen"

Rectangle {
    id: main
    color: activePallete.normalBackground
    property Item appContent: appContentItem

    function switchScreen(from, to) {
        switchScreenAnimation.from = from
        switchScreenAnimation.to = to
        switchScreenAnimation.start()
    }

    KColorScheme {
        id: activePallete
        colorGroup: KColorScheme.Active
        colorSet: KColorScheme.Window
    }

    DataAccess {
        id: dataAccess
    }

    QtObject {
        id: helper
        property string name: ktouch.keyboardLayoutName
        property int keyboardLayoutCount: ktouch.globalDataIndex.keyboardLayoutCount
        property int courseCount: ktouch.globalDataIndex.courseCount
        onNameChanged: {
            activeKeyboardLayout.update()
        }
        onKeyboardLayoutCountChanged: {
            if (ktouch.globalDataIndex.isValid)
                activeKeyboardLayout.update()
        }
    }

    ResourceModel {
        id: resourceModel
        dataIndex: ktouch.globalDataIndex
    }

    ProfileDataAccess {
        id: profileDataAccess
    }

    Preferences {
        id: preferences
    }

    KeyboardLayout {
        id: activeKeyboardLayout

        Component.onCompleted: {
            if (ktouch.globalDataIndex.isValid) {
                activeKeyboardLayout.update()
            }
        }

        function update() {
            isValid = false
            var name = ktouch.keyboardLayoutName;

            // first pass - exact match

            for (var i = 0; i < ktouch.globalDataIndex.keyboardLayoutCount; i++)
            {
                var dataIndexLayout = ktouch.globalDataIndex.keyboardLayout(i)

                if (dataIndexLayout.name === name) {
                    dataAccess.loadKeyboardLayout(dataIndexLayout, activeKeyboardLayout)
                    return
                }
            }

            // second pass - substring match

            for (var i = 0; i < ktouch.globalDataIndex.keyboardLayoutCount; i++)
            {
                var dataIndexLayout = ktouch.globalDataIndex.keyboardLayout(i)

                if (name.search(dataIndexLayout.name) === 0) {
                    dataAccess.loadKeyboardLayout(dataIndexLayout, activeKeyboardLayout)
                    return
                }
            }
        }
    }

    Course {
        id: selectedCourse
        property Lesson selectedLesson
    }

    Lesson {
        id: customLessonCopy
    }

    Item {
        anchors.fill: parent
        id: appContentItem
        layer.enabled: true

        HomeScreen {
            id: homeScreen
            anchors.fill: parent
            activeKeyboardLayoutName: activeKeyboardLayout.isValid? activeKeyboardLayout.name: helper.name
            visible: false
            focus: true
            onLessonSelected: {
                trainingScreen.profile = profile
                var lessonIndex = -1;
                for (var i = 0; i < course.lessonCount; i++) {
                    if (lesson === course.lesson(i)) {
                        lessonIndex = i
                        break
                    }
                }
                selectedCourse.copyFrom(course)

                if (lessonIndex !== -1) {
                    selectedCourse.selectedLesson = selectedCourse.lesson(lessonIndex)
                }
                else {
                    customLessonCopy.copyFrom(lesson)
                    selectedCourse.selectedLesson = customLessonCopy
                }

                main.switchScreen(homeScreen, trainingScreen)
            }
            Component.onCompleted: {
                homeScreen.reset()
                homeScreen.visible = true
                homeScreen.start()
            }
        }

        TrainingScreen {
            id: trainingScreen
            anchors.fill: parent
            visible: false
            keyboardLayout: homeScreen.selectedKeyboardLayout
            course: selectedCourse
            lesson: selectedCourse.selectedLesson
            onRestartRequested: main.switchScreen(trainingScreen, trainingScreen)
            onAbortRequested: main.switchScreen(trainingScreen, homeScreen)
            onFinished: main.switchScreen(trainingScreen, scoreScreen)
        }

        ScoreScreen {
            id: scoreScreen
            anchors.fill: parent
            visible: false
            course: trainingScreen.course
            lesson: trainingScreen.lesson
            stats: trainingScreen.stats
            profile: trainingScreen.profile
            referenceStats: trainingScreen.referenceStats
            onHomeScreenRequested: main.switchScreen(scoreScreen, homeScreen)
            onLessonRepetionRequested: main.switchScreen(scoreScreen, trainingScreen)
            onNextLessonRequested: {
                selectedCourse.selectedLesson = lesson
                main.switchScreen(scoreScreen, trainingScreen)
            }
        }
    }

    Rectangle {
        id: curtain
        anchors.fill: parent
        color: "#000"
        opacity: 0
    }

    SequentialAnimation
    {
        id: switchScreenAnimation
        property Item from
        property Item to
        NumberAnimation {
            target: curtain
            property: "opacity"
            to: 1
            duration: switchScreenAnimation.to == homeScreen? Units.longDuration: 3 * Units.longDuration
            easing.type: Easing.OutQuad
        }
        PropertyAction {
            target: switchScreenAnimation.from
            property: "visible"
            value: false
        }
        ScriptAction {
            script: switchScreenAnimation.to.reset()
        }
        PropertyAction {
            target: switchScreenAnimation.to
            property: "visible"
            value: true
        }
        ScriptAction {
            script: {
                switchScreenAnimation.to.start()
                switchScreenAnimation.to.forceActiveFocus()
            }
        }
        NumberAnimation {
            target: curtain
            property: "opacity"
            to: 0
            duration: switchScreenAnimation.to == homeScreen? Units.longDuration: 3 * Units.longDuration
            easing.type: Easing.InQuad
        }
    }
}
