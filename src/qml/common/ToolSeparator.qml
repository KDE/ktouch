/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Controls 2.2 as Controls
import ktouch 1.0

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
