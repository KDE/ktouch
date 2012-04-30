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

Item {
    id: main

    property DataIndex dataIndex: dataAccess.loadDataIndex()
    property string name: keyboardLayoutName
    property KeyboardLayout keyboardLayout: lookupKeyboardLayout(keyboardLayoutName)
    property variant courses: lookupCourses(keyboardLayout? keyboardLayout.name: "")

    function lookupKeyboardLayout(name)
    {
        for (var i = 0; i < dataIndex.keyboardLayoutCount; i++)
        {
            var dataIndexLayout = dataIndex.keyboardLayout(i)
            if (name.search(dataIndexLayout.name) === 0)
                return dataAccess.loadResourceKeyboardLayout(dataIndexLayout.path)

        }
        return null;
    }

    function lookupCourses(name)
    {
        var courses = [];
        for (var i = 0; i < dataIndex.courseCount; i++)
        {
            var dataIndexCourse = dataIndex.course(i)
            if (name == dataIndexCourse.keyboardLayoutName)
            {
                var course = dataAccess.loadResourceCourse(dataIndexCourse.path)
                courses.push(course)
            }

        }
        return courses;
    }

    function switchScreen(from, to) {
        switchScreenAnimation.from = from
        switchScreenAnimation.to = to
        switchScreenAnimation.start()
    }

    DataAccess {
        id: dataAccess
    }

    ProfileDataAccess {
        id: profileDataAccess
    }

    Preferences {
        id: preferences
    }

    HomeScreen {
        id: homeScreen
        anchors.fill: parent
        courses: main.courses
        visible: false
        focus: true
        onLessonSelected: {
            trainingScreen.profile = profile
            trainingScreen.lesson = lesson
            trainingScreen.course = course
            scoreScreen.profile = profile
            scoreScreen.lesson = lesson
            scoreScreen.course = course
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
        keyboardLayout: main.keyboardLayout
        onRestartRequested: main.switchScreen(trainingScreen, trainingScreen)
        onAbortRequested: main.switchScreen(trainingScreen, homeScreen)
        onFinished: {
            scoreScreen.stats = stats
            main.switchScreen(trainingScreen, scoreScreen)
        }
    }

    ScoreScreen {
        id: scoreScreen
        anchors.fill: parent
        visible: false
        onHomeScreenRequested: main.switchScreen(scoreScreen, homeScreen)
        onLessonRepetionRequested: main.switchScreen(scoreScreen, trainingScreen)
        onNextLessonRequested: {
            trainingScreen.lesson = lesson
            scoreScreen.lesson = lesson
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
            duration: 750
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
        NumberAnimation {
            target: curtain
            property: "opacity"
            to: 0
            duration: 750
            easing.type: Easing.InQuad
        }
        ScriptAction {
            script: {
                switchScreenAnimation.to.start()
                switchScreenAnimation.to.forceActiveFocus()
            }
        }
    }
}
