/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import ktouch 1.0

import "../common"

Item {
    id: root

    Preferences {
        id: preferences
    }

    property real scaleFactor: Math.pow(2, keyboardLayoutEditor.zoomLevel / 2)
    property KeyboardLayout layout: keyboardLayoutEditor.keyboardLayout
    property int lastZIndex: 0

    width: keyContainer.width + 40
    height: keyContainer.height + 40

    MouseArea {
        anchors.fill: parent
        onPressed: {
            if (mouse.button == Qt.LeftButton) {
                keyboardLayoutEditor.selectedKey = null
                mouse.accepted = true
            }
        }
    }

    LineGrid {
        id: keyContainer

        anchors.centerIn: parent
        width: Math.round(layout.width * scaleFactor)
        height: Math.round(layout.height * scaleFactor)

        lineDistance: 10.0 * scaleFactor
        color: "#121286"
        backgroundColor: "#cccccc"

        Repeater {
            id: keys
            model: layout.isValid? layout.keyCount: 0

            KeyItem {
                property bool manipulated: false
                id: keyItem
                keyboardLayout: layout;
                keyIndex: index
                isHighlighted: keyItem.key == keyboardLayoutEditor.selectedKey
                animateHighlight: false
                opacity: manipulated? 0.7: 1.0
                horizontalScaleFactor: scaleFactor
                verticalScaleFactor: scaleFactor

                MouseArea {
                    anchors.fill: parent
                    cursorShape: keyItem.manipulated? Qt.SizeAllCursor: Qt.ArrowCursor
                    onPressed: {
                        if (mouse.button == Qt.LeftButton) {
                            keyboardLayoutEditor.selectedKey = layout.key(index)
                            root.lastZIndex++
                            keyItem.z = root.lastZIndex
                        }
                    }
                    drag {
                        target: !keyboardLayoutEditor.readOnly? keyItem: undefined
                        axis: Drag.XandYAxis
                        minimumX: 0
                        maximumX: keyContainer.width - keyItem.width
                        minimumY: 0
                        maximumY: keyContainer.height - keyItem.height
                        onActiveChanged: {
                            keyItem.manipulated = drag.active
                            if (!drag.active) {
                                var left = 10 * Math.round(keyItem.x / scaleFactor / 10)
                                var top = 10 * Math.round(keyItem.y / scaleFactor / 10)
                                keyboardLayoutEditor.setKeyGeometry(keyIndex, left, top, keyItem.key.width, keyItem.key.height)
                            }
                        }
                    }
                }

                Behavior on opacity {
                    NumberAnimation { duration: 150 }
                }
            }
        }

        SelectionRectangle {
            keyboardLayout: layout;
            target: keyboardLayoutEditor.selectedKey
            z: root.lastZIndex + 1
            interactive: !keyboardLayoutEditor.readOnly
        }
    }
}
