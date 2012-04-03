import QtQuick 1.1
import org.kde.qtextracomponents 0.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

Item {
    id: root
    property alias title: label.text
    property string iconSource
    property alias label: label
    signal selected
    height: padding.height + bg.margins.top + bg.margins.bottom
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
            fill: parent
            topMargin: bg.margins.top
            rightMargin: bg.margins.right
            bottomMargin: bg.margins.bottom
            leftMargin: bg.margins.left
        }

        width: parent.width
        height: Math.max(label.height, label.height)

        QIconItem {
            id: iconItem
            visible: !!root.iconSource
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            icon: QIcon(root.iconSource)
            width: theme.smallIconSize
            height: theme.smallIconSize
        }

        PlasmaComponents.Label {
            id: label
            elide: Text.ElideRight
            anchors {
                left: iconItem.visible? iconItem.right: parent.left
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
            height: paintedHeight
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.selected()
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
