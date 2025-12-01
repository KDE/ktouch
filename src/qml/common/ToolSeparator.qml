/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick
import QtQuick.Controls as Controls
import ktouch

Controls.ToolSeparator {
    id: control

    property alias colorScheme: colorScheme

    KColorScheme {
        id: colorScheme
        colorGroup: KColorScheme.Active
        colorSet: KColorScheme.Complementary
    }
    padding: Units.smallSpacing

    contentItem: Rectangle {
        implicitWidth: control.vertical? 1: Units.gridUnit
        implicitHeight: control.vertical? Units.gridUnit: 1
        color: control.colorScheme.normalText
    }
}
