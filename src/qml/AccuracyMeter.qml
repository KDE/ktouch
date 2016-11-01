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

    property real accuracy: 1.0
    property real referenceAccuracy: 1.0

    label: i18n("Accuracy")
    value: strFormatter.formatAccuracy(meter.accuracy)
    referenceValue: strFormatter.formatAccuracyDiff(meter.referenceAccuracy, meter.accuracy)
    valueStatus: Math.round(1000 * meter.accuracy) >= Math.round(10 * preferences.requiredAccuracy)? "good": "bad"

    analogPartContent: Image {
        anchors.centerIn: parent
        source: utils.findImage("accuracymeter-background.png")

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
            source: utils.findImage("accuracymeter-scale.png")
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
            source: utils.findImage("accuracymeter-hand.png")
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
