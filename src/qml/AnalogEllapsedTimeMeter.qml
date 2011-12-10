import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore

Item
{
    property variant ellapsedTime

    width: 102
    height: 102

    PlasmaCore.Svg {
        id: clockSvg
        imagePath: findImage("ellapsedtimemeter.svgz")
    }

    PlasmaCore.FrameSvgItem {
        anchors.fill: parent
        imagePath: findImage("meterbox.svgz")
        prefix: "analog"
    }

    PlasmaCore.SvgItem {
        anchors.centerIn: parent
        svg: clockSvg
        elementId: "background"
        width: naturalSize.width
        height: naturalSize.height
    }

    PlasmaCore.SvgItem {
        id: minuteHand
        anchors.centerIn: parent
        svg: clockSvg
        elementId: "minute-hand"
        width: naturalSize.width
        height: naturalSize.height
        smooth: true
        transform: Rotation {
            origin.x: minuteHand.naturalSize.width / 2
            origin.y: minuteHand.naturalSize.height / 2
            angle: 6 * getMinutes(ellapsedTime)
            Behavior on angle {
                SpringAnimation { spring: 2; damping: 0.2; modulus: 360; mass: 0.75}
            }
        }
    }

    PlasmaCore.SvgItem {
        id: secondHand
        anchors.centerIn: parent
        svg: clockSvg
        elementId: "second-hand"
        width: naturalSize.width
        height: naturalSize.height
        smooth: true
        transform: Rotation {
            origin.x: secondHand.naturalSize.width / 2
            origin.y: secondHand.naturalSize.height / 2
            angle: 6 * getSeconds(ellapsedTime)
            Behavior on angle {
                SpringAnimation { spring: 2; damping: 0.2; modulus: 360; mass: 0.75}
            }
        }
    }
}
