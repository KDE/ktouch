/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
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
        implicitHeight: layout.implicitHeight + 2 * Units.largeSpacing

        GridLayout {
            id: layout
            anchors.centerIn: parent
            width: root.width - 2 * Units.gridUnit
            rowSpacing: Units.gridUnit
            columnSpacing: Units.gridUnit

            Icon {
                Layout.column: 0
                Layout.row: 0
                height: Units.fontMetrics.roundedIconSize(2 * Units.gridUnit)
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
