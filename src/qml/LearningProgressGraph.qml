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
import ktouch 1.0
import org.kde.ktouch.graph 0.1 as Graph

Rectangle {
    id: item

    property alias model: graph.model

    color: "white"

    border {
        width: 1
        color: "black"
    }

    Column {
        anchors {
            fill: parent
            topMargin: 2 * spacing + legend.height
            leftMargin: spacing
            rightMargin: spacing
            bottomMargin: spacing
        }

        spacing: 20

        Graph.LineGraph {
            id: graph

            width: parent.width
            height: parent.height - legend.height - parent.spacing

            model: learningProgressModel

            dimensions: [
                Graph.Dimension {
                    id: accuracyDimension
                    dataColumn: 5
                    color: "#ffb12d"
                    maximumValue: 1.0
                    label: i18n("Accuracy")
                    unit: "%"
                    unitFactor: 100
                },
                Graph.Dimension {
                    id: charactersPerMinuteDimension
                    dataColumn: 6
                    color: "#38aef4"
                    maximumValue: Math.ceil(model.maxCharactersTypedPerMinute / 120) * 120
                    label: i18n("Characters per Minute")
                }
            ]
        }

        Row {
            id: legend
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20
            Graph.LegendItem {
                dimension: accuracyDimension
            }
            Graph.LegendItem {
                dimension: charactersPerMinuteDimension
            }
        }
    }
}
