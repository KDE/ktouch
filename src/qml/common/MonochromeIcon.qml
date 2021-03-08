/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtGraphicalEffects 1.0

Item {
    property alias icon: icon.icon
    property color color : "#000000"
    width: icon.width
    height: icon.height

    Icon {
        id: icon
        visible: false
    }

    ColorOverlay {
        anchors.fill: icon
        source: icon
        color: parent.color
    }
}

