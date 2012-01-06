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

    DataAccess {
        id: dataAccess
    }

    Preferences {
        id: preferences
    }

    HomeScreen
    {
        id: homeScreen
        anchors.fill: parent
        courses: main.courses
        onLessonSelected: {
            trainingScreen.lesson = lesson
            switchScreenAnimation.from = homeScreen
            switchScreenAnimation.to = trainingScreen
            switchScreenAnimation.start()
        }
    }

    TrainingScreen {
        id: trainingScreen
        anchors.fill: parent
        visible: false
        keyboardLayout: main.keyboardLayout
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
            target: curtain;
            property: "opacity"
            to: 1;
            duration: 750
            easing.type: Easing.OutQuad
        }
        PropertyAction {
            target: switchScreenAnimation.from
            property: "visible"
            value: false
        }
        PropertyAction {
            target: switchScreenAnimation.to
            property: "visible"
            value: true
        }
        NumberAnimation {
            target: curtain;
            property: "opacity"
            to: 0;
            duration: 750
            easing.type: Easing.OutQuad
        }
    }
}
