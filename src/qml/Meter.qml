/*
 *  Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *  Copyright 2015  Sebastian Gottfried <sebastiangottfried@web.de>
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

import QtQuick 2.4
import QtQuick.Layouts 1.1

RowLayout {
    id: meter

    property alias label: label.text
    property alias value: value.text;
    property alias referenceValue: referenceValue.text
    property bool positiveDiffIsGood: true
    property string valueStatus: "none"
    property alias analogPartContent: analogPart.data

    height: 112
    width: 304
    spacing: 0

    BorderImage {
        id: analogPart
        Layout.preferredWidth: height
        Layout.fillHeight: true
        border {
            left: 6
            top: 6
            right:6
            bottom: 6
        }
        source: utils.findImage("meterbox-left.png")
    }

    BorderImage {
        id: digitalPart
        Layout.fillHeight: true
        Layout.fillWidth: true
        border {
            left: 6
            top: 6
            right:6
            bottom: 6
        }
        source: utils.findImage("meterbox-right.png")

        Column {
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 12
                right: parent.right
                rightMargin: 12
            }

            spacing: 0

            Row {
                width: parent.width
                spacing: 5

                Text {
                    id: label
                    width: Math.min(implicitWidth, parent.width - statusLed.width - parent.spacing)
                    color: "#555"
                    font.pixelSize: 15
                    elide: Text.ElideRight
                }

                Rectangle {
                    id: statusLed
                    height: 15
                    width: 15
                    radius: height / 2
                    visible: valueStatus !== "none"
                    color: valueStatus === "good"? "#88ff00": "#424b35"

                    onColorChanged: statusLedAnimaton.restart()

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
                id: referenceValue
                font.pixelSize: 15
                color: {
                    if (text[0] === "+")
                        return positiveDiffIsGood? "#006E28": "#BF0303"
                    if (text[0] === "-")
                        return positiveDiffIsGood? "#BF0303": "#006E28"
                    return "#555"
                }
            }
        }
    }
}
