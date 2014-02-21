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
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.charts 0.1
import ktouch 1.0

LineChart {
    id: chart

    property Dimension accuracy: accuracyDimension
    property Dimension charactersPerMinute: charactersPerMinuteDimension

    pitch: 60

    function minAccuracy(accuracy) {
        var canditades = [0.9, 0.8, 0.5]

        for (var i = 0; i < canditades.length; i++) {
            if (canditades[i] < accuracy) {
                return (canditades[i])
            }
        }

        return 0;
    }
    
    dimensions: [
        Dimension {
            id: accuracyDimension
            dataColumn: 5
            color: "#ffb12d"
            minimumValue: chart.minAccuracy(model.minAccuracy)
            maximumValue: 1.0
            label: i18n("Accuracy")
            unit: "%"
            unitFactor: 100
        },
        Dimension {
            id: charactersPerMinuteDimension
            dataColumn: 6
            color: "#38aef4"
            maximumValue: Math.max(Math.ceil(model.maxCharactersTypedPerMinute / 120) * 120, 120)
            label: i18n("Characters per Minute")
        }
    ]
}
