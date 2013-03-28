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
    id: root

    property alias model: core.model
    property alias dimensions: core.dimensions
    property alias pitch: core.pitch
    property alias pointRadius: core.pointRadius

    property alias backgroundColor: bg.color
    property color textColor: theme.textColor

    signal pointClicked(int row, variant point)
    signal pointEntered(int row, variant point)
    signal pointExited(int row, variant point)

    Rectangle {
        id: bg
        anchors.fill: parent
        color: theme.backgroundColor
    }

    LineGraphCore {
        id: core
        anchors.fill: parent

        Flickable {
            id: flickable
            anchors.fill: parent
            clip: true

            contentHeight: height
            contentWidth: lineBg.width

            property real lastWidth: -1

            onContentWidthChanged: {
                if (contentWidth > width) {
                    contentX = contentWidth - width
                }
            }

            onWidthChanged: {
                if (lastWidth != -1 && contentWidth > width) {
                    contentX = contentX + lastWidth - width

                }
                lastWidth = width
            }

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

                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: root.pointClicked(parent.row, parent)
                                onEntered: root.pointEntered(parent.row, parent)
                                onExited: root.pointExited(parent.row, parent)
                            }
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
            topMargin: core.pointRadius + 2
        }
        backgroundItem: bg
        color: root.textColor
        dimension: core.dimensions.length > 0? core.dimensions[0]: null
        valueFactor: 1
    }

    LineLabel {
        anchors {
            top: parent.top
            left: parent.left
            topMargin: core.pointRadius + (core.height - 2 * core.pointRadius - 4) / 2 + 2
        }
        backgroundItem: bg
        color: root.textColor
        dimension: core.dimensions.length > 0? core.dimensions[0]: null
        valueFactor: 0.5
    }

    LineLabel {
        anchors {
            top: parent.top
            right: parent.right
            topMargin: core.pointRadius + 2
        }
        backgroundItem: bg
        color: root.textColor
        dimension: core.dimensions.length > 1? core.dimensions[1]: null
        valueFactor: 1
    }

    LineLabel {
        anchors {
            top: parent.top
            right: parent.right
            topMargin: core.pointRadius + (core.height - 2 * core.pointRadius) / 2 + 2
        }
        backgroundItem: bg
        color: root.textColor
        dimension: core.dimensions.length > 1? core.dimensions[1]: null
        valueFactor: 0.5
    }

}
