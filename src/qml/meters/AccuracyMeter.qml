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

    property real accuracy: 1.0
    property real referenceAccuracy: 1.0

    label: i18n("Accuracy")
    value: strFormatter.formatAccuracy(meter.accuracy)
    referenceValue: strFormatter.formatAccuracyDiff(meter.referenceAccuracy, meter.accuracy)
    valueStatus: Math.round(1000 * meter.accuracy) >= Math.round(10 * preferences.requiredAccuracy)? "good": "bad"

    analogPartContent: Image {
        anchors.centerIn: parent
        source: "qrc:///ktouch/images/accuracymeter-background.png"

        ScaleBackgroundItem {
            anchors.centerIn: parent
            anchors.verticalCenterOffset: 25
            width: scale.width
            height: scale.height
            startAngle: Math.min(135, Math.max(45, 135 - (preferences.requiredAccuracy - 90) * 9))
            stopAngle: 45
            scaleMarkHeight: 8
            color: "#88ff00";
        }

        Image {
            id: scale
            anchors.centerIn: parent
            anchors.verticalCenterOffset: 25
            source: "qrc:///ktouch/images/accuracymeter-scale.png"
        }

        Text {
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 20
            }
            text: "90"
            font.pixelSize: 10
        }

        Text {
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: 20
            }
            text: "100"
            font.pixelSize: 10
        }

        Image {
            id: hand
            anchors.centerIn: parent
            anchors.verticalCenterOffset: 25
            source: "qrc:///ktouch/images/accuracymeter-hand.png"
            transform: Rotation {
                origin.x: hand.width / 2
                origin.y: hand.height / 2
                angle: Math.min(90, Math.max(0, accuracy - 0.9) * 900)
                Behavior on angle {
                    SpringAnimation { spring: 2; damping: 0.2; modulus: 360; mass: 0.75}
                }
            }
        }
    }
}
