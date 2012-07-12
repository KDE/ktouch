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

    width: childrenRect.width + 40
    height: childrenRect.height + 40

    Rectangle {
        id: keyContainer

        anchors.centerIn: parent
        width: childrenRect.width
        height: childrenRect.height

        color: "#ccc"

        Repeater {
            id: keys
            model: layout.isValid? layout.keyCount: 0

            KeyItem {
                keyboardLayout: layout;
                keyIndex: index
            }
        }
    }
}
