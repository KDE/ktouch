/*
 *  Copyright 2014  Sebastian Gottfried <sebastiangottfried@web.de>
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

import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

Column {
    id: root

    property alias checked: radioButton.checked
    property alias enabled: radioButton.enabled
    property alias label: label.text
    property alias hint: hint.text

    spacing: 10

    Row {
        id: radioButtonRow
        width: parent.width
        spacing: Math.round(label.height / 4)

        PlasmaComponents.RadioButton {
            id: radioButton
            anchors.verticalCenter: parent.verticalCenter
        }

        PlasmaComponents.Label {
            id: label
            anchors.verticalCenter: parent.verticalCenter
            height: paintedHeight
            /*
             * The text wrapping of the label doesn't work if it is invible
             * (wrapped at every character), hence the following hack.
             */
            width: visible? parent.width - radioButton.width - parent.spacing: 0
            wrapMode: Text.Wrap
            opacity: radioButton.opacity
            MouseArea {
                anchors.fill: parent
                enabled: radioButton.enabled
                onClicked: {
                    radioButton.checked = true
                }
            }
        }
    }

    Row {
        width: parent.width
        spacing: radioButtonRow.spacing

        Item {
            width: radioButton.width
            height: hint.height
        }

        PlasmaComponents.Label {
            id: hint
            font.pointSize: theme.smallestFont.pointSize
            font.italic: true
            height: paintedHeight
            width: visible? parent.width - radioButton.width - parent.spacing: 0
            wrapMode: Text.Wrap
            opacity: radioButton.opacity
        }
    }
}
