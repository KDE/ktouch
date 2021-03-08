/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Controls 2.2 as Controls
import ktouch 1.0

GridView {
    id: control

    property alias colorScheme: colorScheme
    property alias background: backgroundItem

    KColorScheme {
        id: colorScheme
        colorGroup: control.enabled? KColorScheme.Active: KColorScheme.Disabled
        colorSet: KColorScheme.View
    }

    Rectangle {
        id: backgroundItem
        anchors.fill: parent
        color: colorScheme.normalBackground
        z: -1
    }

    FocusBar {
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        control: control
    }

    FocusBar {
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        control: control
    }

    Controls.ScrollBar.vertical: ScrollBar { }
}
