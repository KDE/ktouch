/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.quickcharts as Charts
import org.kde.quickcharts.controls
import ktouch

Control {
    id: root

    property var model

    function minAccuracy(accuracy) {
        var canditades = [0.9, 0.8, 0.5]

        for (var i = 0; i < canditades.length; i++) {
            if (canditades[i] < accuracy) {
                return (canditades[i])
            }
        }

        return 0;
    }

    background: Rectangle {
        color: Kirigami.Theme.backgroundColor

        Kirigami.Theme.colorSet: Kirigami.Theme.View
        Kirigami.Theme.inherit: false
    }

    contentItem: Item {
        anchors.fill: parent;

        GridLines {
            id: horizontalLines

            anchors.fill: lineChart

            chart: lineChart

            major.frequency: 2
            major.lineWidth: 2
            major.color: Qt.rgba(0.8, 0.8, 0.8, 1.0)

            minor.frequency: 1
            minor.lineWidth: 1
            minor.color: Qt.rgba(0.8, 0.8, 0.8, 1.0)
        }

        GridLines {
            id: verticalLines

            anchors.fill: lineChart

            chart: lineChart

            direction: GridLines.Vertical;

            major.count: 1
            major.lineWidth: 2
            major.color: Qt.rgba(0.8, 0.8, 0.8, 1.0)

            minor.count: 3
            minor.lineWidth: 1
            minor.color: Qt.rgba(0.8, 0.8, 0.8, 1.0)
        }

        AxisLabels {
            id: yAxisLabelsCharacters

            anchors {
                left: parent.left
                top: parent.top
                bottom: xAxisLabels.top
            }

            direction: AxisLabels.VerticalBottomTop
            delegate: Label { text: AxisLabels.label }
            source: Charts.ChartAxisSource {
                chart: lineChart
                axis: Charts.ChartAxisSource.YAxis
                itemCount: 5
            }
        }

        AxisLabels {
            id: yAxisLabelsAccuracy

            anchors {
                right: parent.right
                top: parent.top
                bottom: xAxisLabels.top
            }

            direction: AxisLabels.VerticalBottomTop
            delegate: Label {
                text: i18nc("percentage", "%1%", Math.round(AxisLabels.label * 100))
            }
            source: Charts.ChartAxisSource {
                chart: accuracyChart
                axis: Charts.ChartAxisSource.YAxis
                itemCount: 5
            }
        }

        AxisLabels {
            id: xAxisLabels

            anchors {
                left: yAxisLabelsCharacters.right
                right: yAxisLabelsAccuracy.left
                bottom: legend.top
            }

            delegate: Label { text: AxisLabels.label }
            source: Charts.ChartAxisSource { chart: lineChart; axis: Charts.ChartAxisSource.XAxis; itemCount: 5 }
        }

        RowLayout {
            id: legend
            anchors {
                left: yAxisLabelsCharacters.right
                right: yAxisLabelsAccuracy.left
                bottom: parent.bottom
                bottomMargin: Kirigami.Theme.smallSpacing
            }

            Component {
                id: legendDelegate

                RowLayout {
                    property var itemData: typeof modelData !== "undefined" ? modelData : model

                    Rectangle {
                        implicitWidth: Theme.smallSpacing
                        color: itemData['color']
                    }

                    Label {
                        text: itemData['name']
                        Layout.fillWidth: true
                    }
                }

            }

            Repeater {
                model: LegendModel { chart: lineChart }
                delegate: legendDelegate
            }

            Repeater {
                model: LegendModel { chart: accuracyChart }
                delegate: legendDelegate
            }
        }

        Charts.LineChart {
            id: lineChart
            anchors {
                top: parent.top
                left: yAxisLabelsCharacters.right
                right: yAxisLabelsAccuracy.left
                bottom: xAxisLabels.visible ? xAxisLabels.top : legend.top
                leftMargin: Units.smallSpacing
                rightMargin: Units.smallSpacing
            }

            xRange.automatic: true
            yRange.automatic: true

            colorSource: Charts.ColorGradientSource {
                baseColor: Kirigami.Theme.highlightColor
                itemCount: lineChart.valueSources.length
            }
            valueSources: Charts.ModelSource {
                roleName: "charactersPerMinute"
                model: root.model ? root.model : null
            }

            nameSource: Charts.ArraySource {
                array: [i18nc("@label chart legend", "Characters per minute")]
            }

            pointDelegate: toolTipPointDelegate
        }

        Charts.LineChart {
            id: accuracyChart
            anchors {
                top: parent.top
                left: yAxisLabelsCharacters.right
                right: yAxisLabelsAccuracy.left
                bottom: xAxisLabels.visible ? xAxisLabels.top : legend.top
                leftMargin: Units.smallSpacing
                rightMargin: Units.smallSpacing
            }

            xRange.automatic: true
            yRange {
                from: root.model ? root.minAccuracy(root.model.minAccuracy) : 0
                to: 1
                automatic: false
            }

            colorSource: Charts.ColorGradientSource {
                baseColor: Kirigami.Theme.positiveTextColor
                itemCount: lineChart.valueSources.length
            }

            valueSources: Charts.ModelSource {
                roleName: "accuracy"
                model: root.model ? root.model : null
            }

            nameSource: Charts.ArraySource {
                array: [i18nc("@label chart legend", "Accuracy")]
            }

            pointDelegate: toolTipPointDelegate
        }
    }

    ToolTip {
        id: toolTip

        property int row: -1
        width: Kirigami.Units.gridUnit * 20
        height: informationTable.implicitHeight + topPadding + bottomPadding

        function findLessonTitle(id) {
            var course = model.courseFilter
            if (course) {
                for (var i = 0; i < course.lessonCount; i++) {
                    if (course.lesson(i).id === id) {
                        return course.lesson(i).title
                    }
                }
            }
            return i18n("<i>Unknown</i>")
        }

        InformationTable {
            id: informationTable
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }
            property list<InfoItem> infoModel: [
                InfoItem {
                    title: i18nc("Statistics on lesson:", "Lesson:")
                    text: toolTip.row !== -1 ? toolTip.findLessonTitle(learningProgressModel.lessonId(toolTip.row)): ""
                },
                InfoItem {
                    title: i18n("Training on:")
                    text: toolTip.row !== -1 ? learningProgressModel.date(toolTip.row).toLocaleString() : ""
                },
                InfoItem {
                    title: i18n("Accuracy:")
                    text: toolTip.row !== -1 ? strFormatter.formatAccuracy(learningProgressModel.accuracy(toolTip.row)) : ""
                },
                InfoItem {
                    title: i18n("Characters per Minute:")
                    text: toolTip.row !== -1 ? learningProgressModel.charactersPerMinute(toolTip.row) : ""
                }
            ]
            model: infoModel
        }
    }

    Component {
        id: toolTipPointDelegate

        Rectangle {
            id: delegate
            color: Charts.LineChart.color
            height: 14
            width: 14
            radius: height

            HoverHandler {
                onHoveredChanged: {
                    if (hovered) {
                        toolTip.row = delegate.Charts.LineChart.index;
                        toolTip.x = delegate.x;
                        toolTip.y = delegate.y + 20;
                        toolTip.open();
                    } else {
                        toolTip.close();
                    }
                }
            }
        }
    }
}
