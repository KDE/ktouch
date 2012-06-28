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
import ktouch 1.0

Item {
    id: meter

    property variant ellapsedTime
    property variant referenceEllapsedTime

    width: childrenRect.width
    height: childrenRect.height

    Row {
        AnalogEllapsedTimeMeter {
            ellapsedTime: meter.ellapsedTime
        }
        DigitalMeter {
            label: i18n("Ellapsed time")
            value: formatTime(meter.ellapsedTime)
            referenceValue: formatTimeDiff(meter.referenceEllapsedTime, meter.ellapsedTime)
            positiveDiffIsGood: false
        }
    }
}
