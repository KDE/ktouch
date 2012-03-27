import QtQuick 1.1
import ktouch 1.0

Item {
    id: main

    property DataIndex dataIndex: dataAccess.loadDataIndex()
    property string name: keyboardLayoutName
    property KeyboardLayout keyboardLayout: lookupKeyboardLayout(keyboardLayoutName)
    property variant courses: lookupCourses(keyboardLayoutName)

    function lookupKeyboardLayout(name)
    {
        for (var i = 0; i < dataIndex.keyboardLayoutCount; i++)
        {
            var dataIndexLayout = dataIndex.keyboardLayout(i)
            if (name == dataIndexLayout.name)
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

    Preferences {
        id: preferences
    }

    HomeScreen {
        id: homeScreen
        anchors.fill: parent
        courses: main.courses
        onLessonSelected: {
            trainingScreen.reset()
            trainingScreen.lesson = lesson
            scoreScreen.lesson = lesson
            scoreScreen.course = course
            main.switchScreen(homeScreen, trainingScreen)
        }
    }

    TrainingScreen {
        id: trainingScreen
        anchors.fill: parent
        visible: false
        keyboardLayout: main.keyboardLayout
        onRestartRequested: main.switchScreen(trainingScreen, trainingScreen)
        onAbortRequested: main.switchScreen(trainingScreen, homeScreen)
        onFinished: main.switchScreen(trainingScreen, scoreScreen)
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
            script: switchScreenAnimation.to.start()
        }
    }
}
