import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore

Item {
    id: root

    property alias content: contentArea.children
    property int innerMargin: 20
    signal opended
    signal closed

    function open() {
        root.state = "open"
        opended()
    }

    function close() {
        root.state = "closed"
        closed()
    }

    function isOpen() {
        return root.state == "open"
    }

    clip: true
    visible: bg.opacity > 0

    /* swallow all mouse events */
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
    }

    Rectangle {
        id: bg
        anchors.fill: parent
        color: theme.backgroundColor
        opacity: 1.0

        Behavior on opacity {
            NumberAnimation {
                duration: 300
            }
        }
    }

    Item {
        id: slider
        width: parent.width
        height: parent.height

        PlasmaCore.FrameSvgItem {
            anchors {
                fill: parent
                leftMargin: 10
                rightMargin: 10
                bottomMargin: 10
            }
            imagePath: "dialogs/background"
            enabledBorders: PlasmaCore.FrameSvg.LeftBorder + PlasmaCore.FrameSvg.RightBorder + PlasmaCore.FrameSvg.BottomBorder

            Item {
                id: contentArea
                anchors {
                    fill: parent
                    leftMargin: parent.margins.left + innerMargin
                    rightMargin: parent.margins.right + innerMargin
                    topMargin: parent.margins.top + innerMargin
                    bottomMargin: parent.margins.bottom + innerMargin
                }
            }
        }
    }

    state: "closed"

    states: [
        State {
            name: "open"
            PropertyChanges { target: bg; opacity: 1.0 }
            PropertyChanges { target: slider; y: 0 }
        },
        State {
            name: "closed"
            PropertyChanges { target: bg; opacity: 0 }
            PropertyChanges { target: slider; y: -slider.parent.height }
        }
    ]

    transitions: [
        Transition {
            from: "*"
            to: "*"
            NumberAnimation { target: slider; property: "y"; duration: 300; easing.type: Easing.InOutQuad }
        }
    ]
}
