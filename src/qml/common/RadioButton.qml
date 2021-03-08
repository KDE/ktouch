/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Controls 2.2
import ktouch 1.0

RadioButton {
    id: control

    hoverEnabled: true
    spacing: Units.largeSpacing
    padding: 0

    property alias colorScheme: colorScheme
    property alias selectionColorScheme: selectionColorScheme
    property alias label: label

    KColorScheme {
        id: colorScheme
        colorGroup: control.enabled? KColorScheme.Active: KColorScheme.Disabled
        colorSet: KColorScheme.Button
    }

    KColorScheme {
        id: selectionColorScheme
        colorGroup: control.enabled? KColorScheme.Active: KColorScheme.Disabled
        colorSet: KColorScheme.Selection
    }

    indicator: Rectangle {
        implicitWidth: control.font.pixelSize
        implicitHeight: control.font.pixelSize
        x: control.leftPadding
        y: parent.height / 2 - height / 2
        radius: height / 2

        border.color: control.checked?
                          Qt.lighter(colorScheme.focusDecoration, control.hovered? 1.3: 1.0):
                          utils.alpha(colorScheme.normalText, control.hovered? 0.9: 0.5)

        color: "#00000000"

        Behavior on border.color {
            ColorAnimation {
                duration: Units.shortDuration
            }
        }

        Rectangle {
            anchors.fill: parent
            anchors.margins: 3
            radius: height / 2
            color: Qt.lighter(colorScheme.focusDecoration, control.hovered? 1.3: 1.0)
            visible: control.checked
        }
    }

    contentItem: Label {
        id: label
        text: control.text
        color: control.colorScheme.normalText
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        topPadding: Units.smallSpacing
        leftPadding: control.indicator.width + control.spacing
        bottomPadding: Units.smallSpacing

        FocusBar {
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                leftMargin: parent.leftPadding
            }
            control: control
        }
    }
}
