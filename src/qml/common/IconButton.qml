/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Controls 2.2
import ktouch 1.0
import QtGraphicalEffects 1.0

Button {
    id: button

    property alias color: content.color
    property alias bgColor: bg.color
    property alias iconName: content.iconName
    property alias colorScheme: buttonColorScheme


    padding: 0

    hoverEnabled: true

    KColorScheme {
        id: buttonColorScheme
        colorGroup:  button.enabled? KColorScheme.Active: KColorScheme.Disabled
        colorSet: KColorScheme.Button
    }

    contentItem: IconLabel {
        color: buttonColorScheme.normalText
        id: content
        text: button.text
        elide: Text.ElideNone

        Behavior on color {
            ColorAnimation { duration: Units.shortDuration }
        }

    }

    background: Item {
        Rectangle {
            anchors.fill: parent;
            id: bg
            color: buttonColorScheme.alternateBackground
            visible: false
        }

        HueSaturation {
            anchors.fill: bg
            source: bg
            saturation: hovered? 0.3: 0
            lightness: hovered? -0.04: 0
            Behavior on saturation {
                NumberAnimation {
                    duration: Units.shortDuration
                }
            }
            Behavior on lightness {
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
