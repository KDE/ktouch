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
