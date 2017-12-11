/*
 *  Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *  Copyright 2015  Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.9
import QtQuick.Controls 2.2
import ktouch 1.0

ItemDelegate {
    id: root
    property string icon
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
                duration: 150

            }
        }
    }

    contentItem: IconLabel {
        id: label
        text: root.text
        icon: root.icon
        color: root.highlighted? listItemColorSchemeHighlighted.activeText: listItemColorSchemeNormal.normalText;
    }
}
