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

    property real scaleFactor: 1.0
    property KeyboardLayout layout: keyboardLayout
    property AbstractKey selKey: selectedKey
    property int lastZIndex: 0

    width: childrenRect.width + 10
    height: childrenRect.height + 10

    MouseArea {
        anchors.fill: parent
        onPressed: {
            if (mouse.button == Qt.LeftButton) {
                selectedKey = null
                mouse.accepted = true
            }
        }
    }

    Rectangle {
        id: keyContainer

        anchors.centerIn: parent
        width: layout.width * scaleFactor
        height: layout.height * scaleFactor

        color: "#ccc"

        Repeater {
            id: keys
            model: layout.isValid? layout.keyCount: 0

            KeyItem {
                id: keyItem
                keyboardLayout: layout;
                keyIndex: index
                isHighlighted: keyItem.key == selectedKey
                animateHighlight: false

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
                            if (!drag.active) {
                                var left = 10 * Math.round(keyItem.x / scaleFactor / 10)
                                var top = 10 * Math.round(keyItem.y / scaleFactor / 10)
                                setKeyGeometry(keyIndex, left, top, keyItem.key.width, keyItem.key.height)
                            }
                        }
                    }
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
