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

import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore

Item {
    property alias label: label.text
    property alias value: value.text;
    property alias referenceValue: referenceValue.text
    property bool positiveDiffIsGood: true
    property string valueStatus: "none"

    width: 192
    height: 112
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
        spacing: 8

        Row {
            spacing: 5

            Text {
                id: label
                color: "#555"
                font.pixelSize: 15
            }

            PlasmaCore.SvgItem {
                id: statusLed
                anchors.verticalCenter: parent.verticalCenter
                elementId: valueStatus === "good"? "led-good": "led-bad"
                svg: PlasmaCore.Svg {
                    imagePath: findImage("statusled.svgz")
                }
                visible: valueStatus !== "none"
                height: naturalSize.height
                width: naturalSize.width
                smooth: true

                onElementIdChanged: statusLedAnimaton.restart()

                SequentialAnimation {
                    id: statusLedAnimaton
                    NumberAnimation { target: statusLed; property: "scale"; to: 1.3; duration: 150; easing.type: Easing.InOutQuad }
                    NumberAnimation { target: statusLed; property: "scale"; to: 1.0; duration: 150; easing.type: Easing.InOutQuad }
                }
            }

        }

        Text {
            id: value
            font.pixelSize: 30
            font.bold: true
        }
        Text {
            function calcColor() {
                if (text[0] === "+")
                    return positiveDiffIsGood? "#006E28": "#BF0303"
                if (text[0] === "-")
                    return positiveDiffIsGood? "#BF0303": "#006E28"
                return "#555"
            }

            id: referenceValue
            font.pixelSize: 15
            color: calcColor()
        }
    }
}
