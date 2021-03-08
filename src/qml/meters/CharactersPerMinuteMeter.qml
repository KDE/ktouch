/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *  SPDX-FileCopyrightText: 2015 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import ktouch 1.0

Meter {
    id: meter

    property int charactersPerMinute: 0
    property int referenceCharactersPerMinute: 0

    property int minimumCharactersPerMinute: preferences.requiredStrokesPerMinute
    property int diff: meter.charactersPerMinute - meter.referenceCharactersPerMinute

    label: i18n("Characters per Minute")
    value: meter.charactersPerMinute
    referenceValue: strFormatter.formatSign(diff) + " " + (diff > 0? diff: -diff)
    valueStatus: meter.charactersPerMinute >= minimumCharactersPerMinute? "good": "bad"

    analogPartContent: Image {
        anchors.centerIn: parent
        source: "qrc:///ktouch/images/charactersperminutemeter-background.png"

        ScaleBackgroundItem {
            anchors.centerIn: parent
            anchors.verticalCenterOffset: 25
            width: scale.width
            height: scale.height
            startAngle: Math.min(135, Math.max(45, 135 - (minimumCharactersPerMinute  * 90 / 360)))
            stopAngle: 45
            scaleMarkHeight: 8
            color: "#88ff00";
        }

        Image {
            id: scale
            anchors.centerIn: parent
            anchors.verticalCenterOffset: 25
            source: "qrc:///ktouch/images/charactersperminutemeter-scale.png"
        }

        Text {
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 20
            }
            text: "0"
            font.pixelSize: 10
        }

        Text {
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: 20
            }
            text: "360"
            font.pixelSize: 10
        }

        Image {
            id: hand
            anchors.centerIn: parent
            anchors.verticalCenterOffset: 25
            source: "qrc:///ktouch/images/charactersperminutemeter-hand.png"
            smooth: true
            transform: Rotation {
                origin.x: hand.width / 2
                origin.y: hand.height / 2
                angle: Math.min(90, charactersPerMinute * 90 / 360)
                Behavior on angle {
                    SpringAnimation { spring: 2; damping: 0.2; modulus: 360; mass: 0.75}
                }
            }
        }
    }
}
