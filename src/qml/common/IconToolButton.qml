/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
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
        elide: Text.ElideNone
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
