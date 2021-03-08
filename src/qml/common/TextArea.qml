/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Controls 2.2 as Controls
import ktouch 1.0

Controls.TextArea {
    id: control

    property KColorScheme colorScheme: KColorScheme {
        colorGroup: control.enabled? KColorScheme.Active: KColorScheme.Disabled
        colorSet:KColorScheme.View
    }

    property KColorScheme selectionColorScheme: KColorScheme {
        colorGroup: control.enabled? KColorScheme.Active: KColorScheme.Disabled
        colorSet:KColorScheme.Selection
    }

    selectByMouse: true
    selectByKeyboard: true
    color: control.colorScheme.normalText
    selectionColor: selectionColorScheme.normalBackground
    selectedTextColor: selectionColorScheme.normalText

    background: Rectangle {
        color: control.colorScheme.normalBackground
        anchors.fill: parent
        border.width: 1
        border.color: control.activeFocus?
                          control.colorScheme.focusDecoration:
                          Qt.hsva(control.colorScheme.focusDecoration.hslHue,
                                  0,
                                  control.colorScheme.focusDecoration.hslValue,
                                  control.enabled? 0.5: 0.2)
        Behavior on border.color {
            ColorAnimation { duration: Units.shortDuration }
        }
    }


}
