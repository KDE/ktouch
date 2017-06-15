/*
 *  Copyright 2014  Sebastian Gottfried <sebastiangottfried@web.de>
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

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

ColumnLayout {
    id: root

    property alias checked: radioButton.checked
    property alias enabled: radioButton.enabled
    property alias label: label.text
    property alias hint: hint.text

    spacing: 10

    Row {
        id: radioButtonRow
        Layout.fillWidth: true
        spacing: Math.round(label.height / 4)

        RadioButton {
            id: radioButton
            anchors.verticalCenter: parent.verticalCenter
        }

        Label {
            id: label
            anchors.verticalCenter: parent.verticalCenter
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
        Layout.fillWidth: true
        spacing: radioButtonRow.spacing

        Item {
            width: radioButton.width
            height: hint.height
        }

        Label {
            id: hint
            font.italic: true
            width: visible? parent.width - radioButton.width - parent.spacing: 0
            wrapMode: Text.Wrap
            opacity: radioButton.opacity
        }
    }
}
