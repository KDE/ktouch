/*
 * Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 1.1

Item {
    property alias model: core.model
    property alias dimensions: core.dimensions
    property alias graphStyle: core.graphStyle
    property alias pitch: core.pitch

    property alias backgroundColor: bg.color

    Rectangle {
        id: bg
        anchors.fill: parent
        color: "white"
    }

    GraphCoreItem
    {
        id: core
        anchors.fill: parent

        Flickable {
            id: test
            anchors.fill: parent
            clip: true

            contentHeight: height
            contentWidth: lineBg.width

            LineGraphBackgroundPainter {
                id: lineBg
                lineGraphCore: core
                height: parent.height
            }

            Repeater {
                id: dimensionsRepeater

                model: core.dimensions.length
                delegate: LineGraphPainter {
                    id: line
                    lineGraphCore: core
                    backgroundPainter: lineBg
                    dimension: index
                    height: parent.height

                    Repeater {
                        model: core.model
                        delegate: LineGraphPoint {
                            lineGraphCore: core
                            backgroundPainter: lineBg
                            dimension: line.dimension
                            row: index
                        }
                    }
                }
            }
        }
    }


}
