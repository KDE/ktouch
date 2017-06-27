import QtQuick 2.4
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.1
import org.kde.kquickcontrolsaddons 2.0

ToolButton {
    id: button

    property color color: "#000000"
    property color backgroundColor: "#ffffff"
    property alias icon: icon.icon

    hoverEnabled: true
    implicitWidth: button.text == ""? implicitHeight: icon.implicitWidth + label.implicitWidth + layout.spacing
    implicitHeight: 40

    contentItem: RowLayout {
        id: layout
        spacing: button.text != ""? 5: 0
        Item {
            Layout.fillHeight: true
            Layout.preferredWidth: icon.width
            Layout.fillWidth: button.text == ""
            QIconItem {
                id: icon
                width: 24
                height: 24
                anchors.centerIn: parent
            }
            ColorOverlay {
                anchors.fill: icon
                source: icon
                color: button.color
            }
        }
        Text {
            id: label
            Layout.fillWidth: true
            color: button.color
            text: button.text
        }
    }

    background: Rectangle {
        opacity: (checked? 0.6: 0) + (hovered? 0.3: 0)
        color: button.backgroundColor
        Behavior on opacity {
            NumberAnimation {
                duration: 150
            }
        }
    }
}
