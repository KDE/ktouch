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
    id: root;

    property KeyboardLayout keyboardLayout
    property variant target: null
    property bool interactive: true

    property variant targetItem: findKeyItem(target)

    anchors.fill: parent
    visible: !!targetItem

    function findKeyItem(key) {
        for (var i = 0; i < keys.count; i++) {
            var keyItem = keys.itemAt(i)
            if (keyItem.key == key) {
                return keyItem
            }
        }
        return null;
    }

    Repeater {
        model: 4
        delegate: SelectionGrip {
            keyboardLayout: root.keyboardLayout
            target: root.target
            targetItem: root.targetItem
            interactive: root.interactive
            horizontalPosition: index % 2 == 0? "left": "right"
            verticalPosition: index < 2? "top": "bottom"
        }
    }
}
