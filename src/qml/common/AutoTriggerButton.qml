/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Controls 2.2
import ktouch 1.0

IconButton {
    id: control

    property alias duration: progressAnimation.duration
    property alias running: progressAnimation.running

    background.children: [
        ProgressBar {
            id: progressBar
            anchors.fill: parent
            value: 0.0
            background: Item {}
            contentItem: Item {
                Rectangle {
                    color: control.colorScheme.focusDecoration
                    opacity: 0.4
                    width: parent.width * progressBar.visualPosition
                    height: parent.height
                }
            }

            NumberAnimation {
                id: progressAnimation
                target: progressBar
                property: "value"
                duration: 10000
                to: 1.0
                running: true
                onStopped: {
                    control.clicked();
                }
            }
        }
    ]
}
