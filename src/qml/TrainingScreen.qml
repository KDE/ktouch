import QtQuick 1.0
import ktouch 1.0

Rectangle {
    id: screen
    color: "#ccc"

    Column {
        anchors.fill: parent
        Rectangle {
            id: head
            color: "#4f4f4f"
            width: parent.width
            height: 60
            Text {
                anchors.centerIn: parent
                color: "#666"
                text: "header"
            }
        }

        TrainingWidget {
            id: body
            width: parent.width
            height: Math.round((parent.height - head.height) / 2)
            onKeyPressed: keyboard.handleKeyPress(event)
            onKeyReleased: keyboard.handleKeyRelease(event)
        }
        Keyboard {
            id: keyboard
            width: parent.width
            height: parent.height - head.height - body.height
        }
    }
}
