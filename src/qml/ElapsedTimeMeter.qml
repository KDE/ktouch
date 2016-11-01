/*
 *  Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
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
        source: utils.findImage("elapsedtimemeter-background.png")

        Image {
            id: minuteHand
            anchors.centerIn: parent
            source: utils.findImage("elapsedtimemeter-minute-hand.png")
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
            source: utils.findImage("elapsedtimemeter-second-hand.png")
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
