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
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

import QtQuick 2.9
import QtQuick.Controls 2.2
import ktouch 1.0

ToolButton {
    id: button

    property alias color: content.color
    property alias iconName: content.iconName
    property color backgroundColor: button.colorScheme.normalBackground

    property KColorScheme colorScheme: KColorScheme {
        id: buttonColorScheme
        colorGroup:  button.enabled? KColorScheme.Active: KColorScheme.Disabled
        colorSet: KColorScheme.Button
    }

    padding: 0

    hoverEnabled: true

    contentItem: IconLabel {
        id: content
        text: button.text
        color: button.colorScheme.normalText
        elide: "ElideNone"
    }

    background: Item {
        Rectangle {
            anchors.fill: parent
            opacity: (button.checked? 0.6: 0) + (button.activeFocus || button.hovered? 0.3: 0)
            color: button.backgroundColor
            Behavior on opacity {
                NumberAnimation {
                    duration: Units.shortDuration
                }
            }
        }

        FocusBar {
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            control: button
        }
    }
}
