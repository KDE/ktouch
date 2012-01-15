import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

Rectangle {
    id: item
    anchors.verticalCenter: parent.verticalCenter
    radius: 3
    color: "#eee4be"
    height: messageLabel.height + 6
    width: messageLabel.width + 6
    smooth: true

    function showMessage(msg){
        item.state = "hidden";
        messageLabel.text = msg
        item.state = "normal"
    }

    function clearMessage() {
        item.state = "cleared"
    }

    function clearMessageImmediately() {
        item.state = "hidden"
    }

    border {
        width: 1
        color: "#decd87"
    }

    transform: Scale {
        id: itemScaler
        origin.x: 0
        origin.y: 0
    }

    state: "hidden"

    PlasmaComponents.Label {
        anchors.centerIn: parent
        id: messageLabel
        text: ""
        height: paintedHeight
        smooth: true
    }

    states: [
        State {
            name: "normal"
            PropertyChanges {
                target: itemScaler
                xScale: 1
            }
        },
        State {
            name: "hidden"
            PropertyChanges {
                target: itemScaler
                xScale: 0
            }
        },
        State {
            name: "cleared"
            PropertyChanges {
                target: itemScaler
                xScale: 0
            }
        }
    ]

    transitions: [
        Transition {
            from: "*"
            to: "normal"
            NumberAnimation {
                target: itemScaler
                property: "xScale"
                duration: 500
                easing.type: Easing.InOutQuad
            }
        },
        Transition {
            from: "normal"
            to: "cleared"
            NumberAnimation {
                target: itemScaler
                property: "xScale"
                duration: 500
                easing.type: Easing.InOutQuad
            }
        }
    ]
}
