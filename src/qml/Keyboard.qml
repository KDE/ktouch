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

Item
{
    id: keyboard

    signal keyboardUpdate

    property KeyboardLayout keyboardLayout
    property int margin: 50
    property real aspectRatio: (keyboardLayout.width + margin) / (keyboardLayout.height + margin)
    property real scaleFactor: Math.min(width / (keyboardLayout.width + margin), height / (keyboardLayout.height + margin))

    function keyItems() {
        var items = []
        for (var i = 0; i < keys.count; i++)
            items.push(keys.itemAt(i))
        return items
    }

    function findKeyItem(keyChar) {
        for (var i = 0; i < keys.count; i++) {
            var key = keys.itemAt(i);
            if (key.match(keyChar))
                return key
        }
        return null

    }

    function handleKeyPress(event) {
        var key = findKeyItem(event)
        if (key)
            key.pressed = true
    }

    function handleKeyRelease(event) {
        var key = findKeyItem(event)
        if (key)
            key.pressed = false
    }

    Item {
        id: keyContainer

        width: childrenRect.width
        height: childrenRect.height
        anchors.centerIn: parent

        Repeater {
            id: keys
            model: keyboard.visible && keyboardLayout.isValid? keyboard.keyboardLayout.keyCount: 0

            onModelChanged: keyboard.keyboardUpdate()

            KeyItem {
                keyboardLayout: keyboard.keyboardLayout;
                keyIndex: index
            }
        }
    }
}
