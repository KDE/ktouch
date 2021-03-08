/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
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
