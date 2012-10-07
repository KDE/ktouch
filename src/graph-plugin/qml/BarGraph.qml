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
import org.kde.plasma.components 0.1 as PlasmaComponents

Item {
    property alias model: core.model
    property alias dimensions: core.dimensions
    property alias pitch: core.pitch

    property alias backgroundColor: bg.color

    Rectangle {
        id: bg
        anchors.fill: parent
        color: "white"
    }

    BarGraphCore {
        id: core
        anchors.fill: parent

        ListView {
            id: list
            anchors.fill: parent
            clip: true

            onContentWidthChanged: {
                // work arround ListView bug: contentWidth doesn't take geometry of the header/footer into account
                if (contentWidth + core.pitch > width) {
                    contentX = contentWidth - width + core.pitch
                }
            }

            model: core.model
            orientation: ListView.Horizontal

            header: Item {
                width: Math.floor(core.pitch / 2)
                height: list.height
            }

            footer: Item {
                width: Math.ceil(core.pitch / 2)
                height: list.height
            }

            delegate: Item {
                id: segmentContainer
                property int row: index

                width: core.pitch
                height: parent.height

                Repeater {
                    anchors.fill: parent
                    model: core.dimensions.length

                    delegate: BarGraphSegment {
                        id: segment
                        barGraphCore: core
                        dimension: index
                        row: segmentContainer.row

                        width: core.barWidth
                        height: parent.height
                        x: (dimension + 0.5) * width

                        Rectangle {
                            property Dimension dimension: core.dimensions[segment.dimension]
                            anchors.bottom: parent.bottom
                            color: dimension.color
                            width: core.barWidth
                            height: segment.barHeight
                        }
                    }
                }
            }
        }

        GraphForegroundPainter {
            anchors.fill: parent
            graphCore: core
            backgroundColor: bg.color
        }
    }

    LineLabel {
        anchors {
            top: parent.top
            left: parent.left
            topMargin: 2
        }
        backgroundItem: bg
        dimension: core.dimensions.length > 0? core.dimensions[0]: null
        valueFactor: 1
    }

    LineLabel {
        anchors {
            top: parent.top
            left: parent.left
            topMargin: (core.height - 4) / 2 + 2
        }
        backgroundItem: bg
        dimension: core.dimensions.length > 0? core.dimensions[0]: null
        valueFactor: 0.5
    }

    LineLabel {
        anchors {
            top: parent.top
            right: parent.right
            topMargin: 2
        }
        backgroundItem: bg
        dimension: core.dimensions.length > 1? core.dimensions[1]: null
        valueFactor: 1
    }

    LineLabel {
        anchors {
            top: parent.top
            right: parent.right
            topMargin: (core.height - 4) / 2 + 2
        }
        backgroundItem: bg
        dimension: core.dimensions.length > 1? core.dimensions[1]: null
        valueFactor: 0.5
    }

}
