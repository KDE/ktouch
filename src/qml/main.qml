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
import ktouch 1.0

import "./common"
import "./meters"
import "./homescreen"
import "./trainingscreen"
import "./scorescreen"

Rectangle {
    SystemPalette {
        id: activePallete
        colorGroup: SystemPalette.Active
    }

    id: main
    color: activePallete.window

    function switchScreen(from, to) {
        switchScreenAnimation.from = from
        switchScreenAnimation.to = to
        switchScreenAnimation.start()
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
            keyboardLayout.update()
        }
        onKeyboardLayoutCountChanged: {
            if (ktouch.globalDataIndex.isValid)
                keyboardLayout.update()
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
        id: keyboardLayout

        Component.onCompleted: {
            if (ktouch.globalDataIndex.isValid) {
                keyboardLayout.update()
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
                    dataAccess.loadKeyboardLayout(dataIndexLayout, keyboardLayout)
                    return
                }
            }

            // second pass - substring match

            for (var i = 0; i < ktouch.globalDataIndex.keyboardLayoutCount; i++)
            {
                var dataIndexLayout = ktouch.globalDataIndex.keyboardLayout(i)

                if (name.search(dataIndexLayout.name) === 0) {
                    dataAccess.loadKeyboardLayout(dataIndexLayout, keyboardLayout)
                    return
                }
            }
        }
    }

    CategorizedResourceSortFilterProxyModel {
        id: availableCourseModel
        resourceModel: resourceModel
        resourceTypeFilter: ResourceModel.CourseItem
        keyboardLayoutNameFilter: keyboardLayout.isValid? keyboardLayout.name: ktouch.keyboardLayoutName
    }

    Course {
        id: selectedCourse
        property Lesson selectedLesson
    }

    Lesson {
        id: customLessonCopy
    }

    HomeScreen {
        id: homeScreen
        anchors.fill: parent
        courseModel: availableCourseModel
        keyboardLayout: keyboardLayout
        keyboardLayoutName: keyboardLayout.isValid? keyboardLayout.name: helper.name
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
        }
    }

    TrainingScreen {
        id: trainingScreen
        anchors.fill: parent
        visible: false
        keyboardLayout: keyboardLayout
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
            duration: switchScreenAnimation.to == homeScreen? 250: 750
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
            duration: switchScreenAnimation.to == homeScreen? 250: 750
            easing.type: Easing.InQuad
        }
    }
}
