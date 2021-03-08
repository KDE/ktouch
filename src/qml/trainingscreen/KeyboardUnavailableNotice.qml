/*
 *  SPDX-FileCopyrightText: 2018 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import ktouch 1.0

import '../common'

Item {
    id: root

    property KColorScheme colorScheme: KColorScheme {}

    height: label.height + 20

    IconLabel {
        id: label
        anchors.centerIn: parent
        width: parent.width - 60
        iconName: "dialog-warning"
        text: i18n("No visualization available for your keyboard layout.")
        color: colorScheme.normalText
    }
}
