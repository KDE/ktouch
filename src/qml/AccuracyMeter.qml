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

import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore

Item {
    id: meter

    property real accuracy: 1.0
    property real referenceAccuracy: 1.0

    property real minimumAccuracy: preferences.requiredAccuracy / 100.0

    width: childrenRect.width
    height: childrenRect.height

    Row {
        AnalogAccuracyMeter {
            accuracy: meter.accuracy
            minimumAccuracy: meter.minimumAccuracy
        }
        DigitalMeter {
            label: i18n("Hit ratio")
            value: formatAccuracy(meter.accuracy)
            referenceValue: formatAccuracyDiff(meter.referenceAccuracy, meter.accuracy)
        }
    }
}
