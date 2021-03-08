/*
 *  SPDX-FileCopyrightText: 2013 Sebastian Gottfried <sebastiangottfried@web.de>
 *  SPDX-FileCopyrightText: 2015 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9

Item {

    id: root

    property alias content: contentArea.data
    property alias color: background.color

    width: contentArea.width + Units.largeSpacing
    height: contentArea.height + Units.largeSpacing


    Rectangle {
        anchors.fill: parent
        id: background
        color: activePalette.mid
        opacity: 0.7
        radius: Units.smallSpacing

        SystemPalette {
            id: activePalette
            colorGroup: SystemPalette.Active
        }
    }

    Behavior on opacity {
        NumberAnimation { duration: Units.shortDuration }
    }

    Row {
        id: contentArea
        anchors.centerIn: parent
        height: childrenRect.height
        spacing: Units.smallSpacing
    }
}
