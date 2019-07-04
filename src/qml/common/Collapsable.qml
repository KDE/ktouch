/*
 *  Copyright 2017  Sebastian Gottfried <sebastian.gottfried@posteo.de>
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

import QtQuick 2.9
import QtQuick.Controls 2.2 as Controls
import ktouch 1.0

Controls.Control {
    id: root

    property bool collapsed: false
    default property Item data

    signal contentReadyForSwap

    function swapContent() {
        contentItem.isSwapping = true
    }

    property KColorScheme colorScheme: KColorScheme {
        colorGroup: KColorScheme.Active
        colorSet: KColorScheme.Window
    }

    height: collapsed && contentItem.opacity == 0? 0: contentItem.implicitHeight
    visible: height > 0

    Behavior on height {
        NumberAnimation {
            duration: 150
            easing.type: Easing.InOutQuad
        }
    }

    background: Rectangle {
        color: colorScheme.normalBackground
    }

    contentItem: Item {
        property bool isSwapping: false
        opacity: !root.collapsed && !isSwapping && root.height === implicitHeight? 1: 0
        data: root.data
        implicitHeight: children.length > 0? children[0].implicitHeight: 0

        Behavior on opacity {
            NumberAnimation {
                duration: 150
            }
        }

        onOpacityChanged: {
            if (isSwapping && opacity === 0) {
                contentReadyForSwap()
                isSwapping = false;
            }
        }
    }
}
