/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Controls 2.2 as Controls
import ktouch 1.0

Controls.ScrollBar {
    id: control

    property alias colorScheme: colorScheme

    KColorScheme {
        id: colorScheme
        colorGroup: control.enabled? KColorScheme.Active: KColorScheme.Disabled
        colorSet: KColorScheme.Window
    }

    contentItem: Rectangle {
        property int size: Math.floor(Units.gridUnit / 3)
        implicitWidth:  control.orientation == Qt.Horizontal? 100: size
        implicitHeight: control.orientation == Qt.Horizontal? size: 100
        radius: size / 2
        color: control.pressed ? colorScheme.focusDecoration: colorScheme.alternateBackground
        opacity: control.active? 1: 0.3
        Behavior on opacity {
            NumberAnimation {
                duration: Units.shortDuration
            }
        }
        Behavior on color {
            ColorAnimation {
                duration: Units.shortDuration
            }
        }
   }
}
