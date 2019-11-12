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
import QtQuick.Layouts 1.3
import ktouch 1.0

import "../common"

Collapsable {
    id: root
    property string activeKeyboardLayoutName: ''

    background: Rectangle {
        color: colorScheme.neutralBackground
    }

    Item {
        implicitWidth: root.width
        implicitHeight: layout.implicitHeight + 20

        GridLayout {
            id: layout
            anchors.centerIn: parent
            width: root.width - 40
            rowSpacing: label.font.pixelSize
            columnSpacing: 10

            Icon {
                Layout.column: 0
                Layout.row: 0
                width: 32
                height: 32
                icon: "dialog-warning"
            }

            Label {
                id: label
                Layout.column: 1
                Layout.row: 0
                Layout.fillWidth: true
                font.bold: true
                wrapMode: Text.Wrap
                text: root.activeKeyboardLayoutName == 'unknown'?
                   i18n("The selected course is intended for a specific keyboard layout."):
                   i18n("The selected course is not intended for your computer's active keyboard layout.")
            }
            Label {
                Layout.column: 1
                Layout.row: 1
                Layout.columnSpan: configureKeyboardButton.visible? 2: 1
                Layout.fillWidth: true
                wrapMode: Text.Wrap
                text: i18n("KTouch can't switch or set up keyboard layouts. Before training, you have to configure your computer to use the correct keyboard layout. Otherwise you will have to use your current keyboard layout to type the lesson text.")
                opacity: 0.7
            }

            IconButton {
                id: configureKeyboardButton
                Layout.column: 2
                Layout.row: 0
                visible: ktouch.keyboardKCMAvailable
                text: i18n("Configure Keyboard")
                onClicked: {
                    ktouch.showKeyboardKCM();
                }
            }
        }
    }
}
