import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import ktouch 1.0

Item {
    id: item

    property Course course
    property int lessonIndex
    signal selected

    property Lesson lesson: item.course.lesson(item.lessonIndex)

    height: padding.height


    state: ListView.isCurrentItem? "selected": mouseArea.containsMouse? "hover": "normal"

    PlasmaCore.FrameSvgItem {
        id: bg
        imagePath: "widgets/viewitem"
        prefix: "hover"
        opacity: 0
        anchors.fill: parent
    }

    Item {
        id: padding

        anchors {
            top: parent.top
            left: parent.left
            topMargin: bg.margins.top
            rightMargin: bg.margins.right
            bottomMargin: bg.margins.bottom
            leftMargin: bg.margins.left
        }

        width: parent.width
        height: childrenRect.height + anchors.topMargin + anchors.leftMargin

        PlasmaComponents.Label {
            text: lesson.title
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: item.selected()
    }

    states: [
        State {
            name: "normal"
            PropertyChanges {
                target: bg
                opacity: 0
            }
        },
        State {
            name: "hover"
            PropertyChanges {
                target: bg
                opacity: 1
                prefix: "hover"
            }
        },
        State {
            name: "selected"
            PropertyChanges {
                target: bg
                opacity: 1
                prefix: "selected"
            }
        }
    ]

    transitions: [
        Transition {
            from: "normal"
            to: "hover"
            NumberAnimation {
                target: bg
                property: "opacity"
                duration: 250
                easing.type: Easing.OutCubic
            }
        },
        Transition {
            from: "hover"
            to: "normal"
            NumberAnimation {
                target: bg
                property: "opacity"
                duration: 250
                easing.type: Easing.OutCubic
            }
        }
    ]
}
