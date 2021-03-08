/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Controls 2.2

Label {
    property string iconName: ""
    property bool reserveSpaceForIcon: false
    id: label
    elide: Text.ElideRight

    padding: Units.largeSpacing
    leftPadding: (iconItem.visible || reserveSpaceForIcon? padding + iconItem.width: 0) + (label.text !== ""? padding: 0)
    verticalAlignment: Text.AlignVCenter

    MonochromeIcon {
        id: iconItem
        visible: label.iconName != ""
        color: label.color
        anchors {
            left: parent.left
            leftMargin: label.text === ""? (label.width - width) / 2: label.padding
            verticalCenter: parent.verticalCenter
        }
        icon: label.iconName
    }
}
