/*
 *  Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore
import Effects 1.0
import ktouch 1.0

Item {
    property int charactersPerMinute: 0
    property int minimumCharactersPerMinute: 180

    width: 102
    height: 102

    PlasmaCore.Svg {
        id: clockSvg
        imagePath: findImage("charactersperminutemeter.svgz")
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

    ScaleBackgroundItem {
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 25
        width: scale.width
        height: scale.height
        startAngle: Math.min(135, Math.max(45, 135 - (minimumCharactersPerMinute  * 90 / 360)))
        stopAngle: 45
        scaleMarkHeight: 8
        color: "#88ff00";
    }

    PlasmaCore.SvgItem {
        id: scale
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 25
        svg: clockSvg
        elementId: "scale"
        width: naturalSize.width
        height: naturalSize.height
    }

    Text {
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 20
        }
        text: "0"
        font.pixelSize: 10
    }

    Text {
        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
            rightMargin: 20
        }
        text: "360"
        font.pixelSize: 10
    }

    PlasmaCore.SvgItem {
        id: hand
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 25
        svg: clockSvg
        elementId: "hand"
        width: naturalSize.width
        height: naturalSize.height
        smooth: true
        effect: DropShadow {
            color: "#000000"
            blurRadius: 5
            xOffset: 0
            yOffset: 0
        }
        transform: Rotation {
            origin.x: hand.naturalSize.width / 2
            origin.y: hand.naturalSize.height / 2
            angle: Math.min(90, charactersPerMinute * 90 / 360)
            Behavior on angle {
                SpringAnimation { spring: 2; damping: 0.2; modulus: 360; mass: 0.75}
            }
        }
    }
}
