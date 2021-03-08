/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *  SPDX-FileCopyrightText: 2015 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Controls 2.2
import ktouch 1.0

ItemDelegate {
    id: root
    property string iconName
    property alias label: label
    property alias bg: bg
    property alias reserveSpaceForIcon: label.reserveSpaceForIcon
    hoverEnabled: true
    padding: 0

    KColorScheme {
        id: listItemColorSchemeNormal
        colorGroup: KColorScheme.Active
        colorSet: KColorScheme.View
    }

    KColorScheme {
        id: listItemColorSchemeHighlighted
        colorGroup: KColorScheme.Active
        colorSet: KColorScheme.Selection
    }

    background: Rectangle {
        id: bg
        anchors.fill: parent
        color: listItemColorSchemeHighlighted.normalBackground
        opacity: root.highlighted ? 1 : (root.hovered? 0.3: 0)
        Behavior on opacity {
            NumberAnimation {
                duration: Units.shortDuration

            }
        }
    }

    contentItem: IconLabel {
        id: label
        text: root.text
        iconName: root.iconName
        color: root.highlighted? listItemColorSchemeHighlighted.activeText: listItemColorSchemeNormal.normalText;
    }
}
