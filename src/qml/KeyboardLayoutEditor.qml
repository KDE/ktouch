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
import ktouch 1.0

Item {
    id: root

    Preferences {
        id: preferences
    }

    property real scaleFactor: Math.pow(2, zoomLevel / 2)
    property real horizontalScaleFactor: scaleFactor
    property real verticalScaleFactor: scaleFactor
    property KeyboardLayout layout: keyboardLayout
    property int lastZIndex: 0

    width: keyContainer.width + 40
    height: keyContainer.height + 40

    MouseArea {
        anchors.fill: parent
        onPressed: {
            if (mouse.button == Qt.LeftButton) {
                selectedKey = null
                mouse.accepted = true
            }
        }
    }

    Grid {
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
                isHighlighted: keyItem.key == selectedKey
                animateHighlight: false
                opacity: manipulated? 0.7: 1.0

                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        if (mouse.button == Qt.LeftButton) {
                            selectedKey = layout.key(index)
                            root.lastZIndex++
                            keyItem.z = root.lastZIndex
                        }
                    }
                    drag {
                        target: !readOnly? keyItem: undefined
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
                                setKeyGeometry(keyIndex, left, top, keyItem.key.width, keyItem.key.height)
                            }
                        }
                    }
                }

                CursorShapeArea {
                  anchors.fill: parent
                  cursorShape: manipulated? Qt.SizeAllCursor: Qt.ArrowCursor
                }

                Behavior on opacity {
                    NumberAnimation { duration: 150 }
                }
            }
        }

        SelectionRectangle {
            keyboardLayout: layout;
            target: selectedKey
            z: root.lastZIndex + 1
            interactive: !readOnly
        }
    }
}
