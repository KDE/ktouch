/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Controls 2.2 as Controls
import ktouch 1.0

Controls.ToolTip {
    id: control

    property alias colorScheme: colorScheme

    KColorScheme {
        id: colorScheme
        colorGroup: KColorScheme.Active
        colorSet: KColorScheme.Complementary
    }

    contentItem: Text {
        text: control.text
        font: control.font
        color: colorScheme.normalText
    }

    background: Rectangle {
        radius: height / 6
        color: control.colorScheme.normalBackground
    }
}
