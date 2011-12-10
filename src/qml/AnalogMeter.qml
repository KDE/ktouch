import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore

Item
{
    width: 102
    height: 102
    PlasmaCore.FrameSvgItem {
        anchors.fill: parent
        imagePath: findImage("meterbox.svgz")
        prefix: "analog"
    }
}
