/*
 *  Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
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

import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import Effects 1.0
import ktouch 1.0

Meter {
    id: meter

    property variant elapsedTime
    property variant referenceElapsedTime

    label: i18n("Elapsed time")
    value: strFormatter.formatTime(meter.elapsedTime)
    referenceValue: strFormatter.formatTimeDiff(meter.referenceElapsedTime, meter.elapsedTime)
    positiveDiffIsGood: false

    analogPartContent: PlasmaCore.SvgItem {
        anchors.centerIn: parent
        svg: clockSvg
        elementId: "background"
        width: naturalSize.width
        height: naturalSize.height

        PlasmaCore.SvgItem {
            id: minuteHand
            anchors.centerIn: parent
            svg: clockSvg
            elementId: "minute-hand"
            width: naturalSize.width
            height: naturalSize.height
            smooth: true
            effect: DropShadow {
                color: "#000000"
                blurRadius: 5
                xOffset: 0
                yOffset: 0
            }
            transform: Rotation {
                origin.x: minuteHand.naturalSize.width / 2
                origin.y: minuteHand.naturalSize.height / 2
                angle: elapsedTime? 6 * getMinutesOfQTime(elapsedTime): 0
                Behavior on angle {
                    SpringAnimation { spring: 2; damping: 0.2; modulus: 360; mass: 0.75}
                }
            }
        }

        PlasmaCore.SvgItem {
            id: secondHand
            anchors.centerIn: parent
            svg: clockSvg
            elementId: "second-hand"
            width: naturalSize.width
            height: naturalSize.height
            smooth: true
            effect: DropShadow {
                color: "#000000"
                blurRadius: 5
                xOffset: 0
                yOffset: 0
            }
            transform: Rotation {
                origin.x: secondHand.naturalSize.width / 2
                origin.y: secondHand.naturalSize.height / 2
                angle: elapsedTime? 6 * getSecondsOfQTime(elapsedTime): 0
                Behavior on angle {
                    SpringAnimation { spring: 2; damping: 0.2; modulus: 360; mass: 0.75}
                }
            }
        }
    }

    PlasmaCore.Svg {
        id: clockSvg
        imagePath: findImage("elapsedtimemeter.svgz")
    }
}
