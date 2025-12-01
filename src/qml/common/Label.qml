/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick
import QtQuick.Controls as Controls
import ktouch

Controls.Label {
    id: control

    property alias colorScheme: colorScheme

    KColorScheme {
        id: colorScheme
        colorGroup: control.enabled? KColorScheme.Active: KColorScheme.Disabled
        colorSet: KColorScheme.Window
    }

    color: colorScheme.normalText
}
