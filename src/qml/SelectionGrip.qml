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
import ktouch 1.0
import Effects 1.0

Item {
    id: root

    property KeyboardLayout keyboardLayout
    property variant target: null
    property variant targetItem: null
    property string horizontalPosition
    property string verticalPosition
    property bool interactive

    property int minSize: 20

    x: targetItem?
        targetItem.x + (horizontalPosition == "left"? 0: targetItem.width - 1):
        0

    y: targetItem?
        targetItem.y + (verticalPosition == "top"? 0: targetItem.height - 1):
        0

    onXChanged: {
        if (mouseArea.drag.active) {
            var effX = x / scaleFactor;
            if (horizontalPosition == "left") {
                setKeyGeometry(targetItem.keyIndex, effX, target.top, target.width + target.left - effX, target.height);
            }
            else {
                setKeyGeometry(targetItem.keyIndex, target.left, target.top, effX - target.left, target.height);
            }
        }
    }

    onYChanged: {
        if (mouseArea.drag.active) {
            var effY = y / scaleFactor;
            if (verticalPosition == "top") {
                setKeyGeometry(targetItem.keyIndex, target.left, effY, target.width, target.height + target.top - effY);
            }
            else {
                setKeyGeometry(targetItem.keyIndex, target.left, target.top, target.width, effY - target.top);
            }
        }
    }

    width: 1
    height: 1

    PlasmaCore.SvgItem {
        anchors.centerIn: parent
        width: 15
        height: 15
        svg: PlasmaCore.Svg {
            imagePath: findImage("resizegrip.svgz")
            usingRenderingCache: false
        }
        elementId: "grip"
        visible: interactive

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                mouse.accepted = true
            }
            drag {
                target: root
                axis: Drag.XandYAxis
                minimumX: horizontalPosition == "left"? 0: targetItem.x + scaleFactor * minSize
                maximumX: horizontalPosition == "left"? targetItem.x + targetItem.width - minSize: keyboardLayout.width * scaleFactor
                minimumY: verticalPosition == "top"? 0: targetItem.y + scaleFactor * minSize
                maximumY: verticalPosition == "top"? targetItem.y + targetItem.height - minSize: keyboardLayout.height * scaleFactor
                onActiveChanged: {
                    if (!drag.active) {
                        var left = 10 * Math.round(target.left / 10);
                        var top = 10 * Math.round(target.top / 10);
                        var width = 10 * Math.round(target.width / 10);
                        var height = 10 * Math.round(target.height / 10);
                        setKeyGeometry(targetItem.keyIndex, left, top, width, height);
                    }
                }
            }
        }
    }
}