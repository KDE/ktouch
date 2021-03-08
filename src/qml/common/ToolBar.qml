/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Controls 2.2 as Controls
import ktouch 1.0

Controls.ToolBar {
    id: control

    property alias colorScheme: colorScheme
    property real dimFactor: 1.3

    background: Rectangle {
        color: colorScheme.toolbarBackground
    }

    KColorScheme {
        id: colorScheme
        colorGroup: KColorScheme.Active
        colorSet: KColorScheme.Complementary
        property color toolbarBackground: Qt.darker(colorScheme.shade(colorScheme.hoverDecoration, KColorScheme.MidShade, colorScheme.contrast, -0.2), control.dimFactor)
    }
}
