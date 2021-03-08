/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
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
            duration: Units.shortDuration
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
        implicitHeight: children.length > 0?
            children[0].implicitHeight + children[0].anchors.topMargin + children[0].anchors.topMargin:
            0

        Behavior on opacity {
            NumberAnimation {
                duration: Units.shortDuration
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
