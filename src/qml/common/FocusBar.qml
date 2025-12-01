/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick
import QtQuick.Controls as Controls
import ktouch

Rectangle {
    property Item control

    height: Math.max(Math.floor(Units.gridUnit / 6), 1)
    color: control.colorScheme.focusDecoration
    opacity: control.activeFocus? 1: 0

    Behavior on opacity {
        NumberAnimation {
            duration: 150
        }
    }
}
