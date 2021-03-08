/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import ktouch 1.0

Item {
    id: root

    property KeyboardLayout keyboardLayout
    property variant target: null
    property variant targetItem: null
    property string horizontalPosition
    property string verticalPosition
    property bool interactive

    property bool xLocked: false
    property bool yLocked: false

    property int minSize: Units.gridUnit

    x: targetItem?
        targetItem.x + (horizontalPosition == "left"? 0: targetItem.width - 1):
        0

    y: targetItem?
        targetItem.y + (verticalPosition == "top"? 0: targetItem.height - 1):
        0

    onXChanged: {
        if (!xLocked && mouseArea.drag.active) {
            xLocked = true
            var effX = Math.round(x / scaleFactor)
            if (horizontalPosition == "left") {
                keyboardLayoutEditor.setKeyGeometry(targetItem.keyIndex, effX, target.top, target.width + target.left - effX, target.height)
            }
            else {
                keyboardLayoutEditor.setKeyGeometry(targetItem.keyIndex, target.left, target.top, effX - target.left, target.height)
            }
            xLocked = false
        }
    }

    onYChanged: {
        if (!yLocked && mouseArea.drag.active) {
            yLocked = true
            var effY = Math.round(y / scaleFactor)
            if (verticalPosition == "top") {
                keyboardLayoutEditor.setKeyGeometry(targetItem.keyIndex, target.left, effY, target.width, target.height + target.top - effY)
            }
            else {
                keyboardLayoutEditor.setKeyGeometry(targetItem.keyIndex, target.left, target.top, target.width, effY - target.top)
            }
            yLocked = false
        }
    }

    width: 1
    height: 1

    SystemPalette {
        id: palette
        colorGroup: SystemPalette.Active
    }

    Rectangle {
        anchors {
            centerIn: parent
            verticalCenterOffset: verticalPosition == "top"? -Units.largeSpacing: Units.largeSpacing
            horizontalCenterOffset: horizontalPosition == "left"? -Units.largeSpacing: Units.largeSpacing
        }
        width: Math.floor(Units.gridUnit / 2)
        height: Math.floor(Units.gridUnit / 2)
        color: palette.highlight
        visible: interactive


        MouseArea {
            id: mouseArea
            anchors.fill: parent
            cursorShape: ((horizontalPosition == "left") != (verticalPosition == "top"))? Qt.SizeBDiagCursor: Qt.SizeFDiagCursor
            onClicked: {
                mouse.accepted = true
            }
            drag {
                target: root
                axis: Drag.XandYAxis
                minimumX: !targetItem? 0: horizontalPosition == "left"? 0: targetItem.x + scaleFactor * minSize
                maximumX: !targetItem? 0: horizontalPosition == "left"? targetItem.x + targetItem.width - minSize: keyboardLayout.width * scaleFactor
                minimumY: !targetItem? 0: verticalPosition == "top"? 0: targetItem.y + scaleFactor * minSize
                maximumY: !targetItem? 0: verticalPosition == "top"? targetItem.y + targetItem.height - minSize: keyboardLayout.height * scaleFactor
                onActiveChanged: {
                    targetItem.manipulated = drag.active
                    if (!drag.active) {
                        var left = 10 * Math.round(target.left / 10)
                        var top = 10 * Math.round(target.top / 10)
                        var width = 10 * Math.round(target.width / 10)
                        var height = 10 * Math.round(target.height / 10)
                        keyboardLayoutEditor.setKeyGeometry(targetItem.keyIndex, left, top, width, height)
                    }
                }
            }
        }
    }
}
