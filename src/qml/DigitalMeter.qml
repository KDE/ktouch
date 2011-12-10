import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore

Item
{
    property alias label: label.text
    property alias value: value.text;
    property alias oldValue: oldValue.text

    width: 202
    height: 102
    PlasmaCore.FrameSvgItem {
        id: digitalMeter
        imagePath: findImage("meterbox.svgz")
        prefix: "digital"
        anchors.fill: parent
    }
    Column {
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 15
        }
        spacing: 3

        Text {
            id: label
            color: "#555"
            font.pixelSize: 15
        }
        Text {
            id: value
            font.pixelSize: 30
            font.bold: true
        }
        Text {
            id: oldValue
            font.pixelSize: 15
            color: "#555"
        }
    }
}
