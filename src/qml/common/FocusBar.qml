/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Controls 2.2 as Controls
import ktouch 1.0

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
