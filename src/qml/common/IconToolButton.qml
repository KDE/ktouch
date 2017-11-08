/*
 *  Copyright 2017  Sebastian Gottfried <sebastian.gottfried@posteo.de>
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

import QtQuick 2.4
import QtQuick.Controls 2.2

ToolButton {
    id: button

    property alias color: content.color
    property alias icon: content.icon
    property color backgroundColor: "#ffffff"
    padding: 0

    hoverEnabled: true

    contentItem: IconLabel {
        id: content
        text: button.text
        elide: "ElideNone"
    }

    background: Rectangle {
        opacity: (checked? 0.6: 0) + (hovered? 0.3: 0)
        color: button.backgroundColor
        Behavior on opacity {
            NumberAnimation {
                duration: 150
            }
        }
    }
}
