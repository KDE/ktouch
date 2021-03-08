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

    property variant elapsedTime
    property variant referenceElapsedTime

    label: i18n("Elapsed time")
    value: strFormatter.formatTime(meter.elapsedTime)
    referenceValue: strFormatter.formatTimeDiff(meter.referenceElapsedTime, meter.elapsedTime)
    positiveDiffIsGood: false

    analogPartContent: Image {
        anchors.centerIn: parent
        source: "qrc:///ktouch/images/elapsedtimemeter-background.png"

        Image {
            id: minuteHand
            anchors.centerIn: parent
            source: "qrc:///ktouch/images/elapsedtimemeter-minute-hand.png"
            smooth: true
            transform: Rotation {
                origin.x: minuteHand.width / 2
                origin.y: minuteHand.height / 2
                angle: elapsedTime? 6 * utils.getMinutesOfQTime(elapsedTime): 0
                Behavior on angle {
                    SpringAnimation { spring: 2; damping: 0.2; modulus: 360; mass: 0.75}
                }
            }
        }

        Image {
            id: secondHand
            anchors.centerIn: parent
            source: "qrc:///ktouch/images/elapsedtimemeter-second-hand.png"
            transform: Rotation {
                origin.x: secondHand.width / 2
                origin.y: secondHand.height / 2
                angle: elapsedTime? 6 * utils.getSecondsOfQTime(elapsedTime): 0
                Behavior on angle {
                    SpringAnimation { spring: 2; damping: 0.2; modulus: 360; mass: 0.75}
                }
            }
        }
    }
}
