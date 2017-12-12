/*
 *  Copyright 2017  Sebastian Gottfried <sebastian.gottfried@posteo.de>
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

import QtQuick 2.9
import QtQuick.Layouts 1.3
import ktouch 1.0
import org.kde.charts 0.1 as Charts

import '../common'

PopupDialog {
    id: root

    property alias course: learningProgressModel.courseFilter
    property alias lesson: learningProgressModel.lessonFilter
    property alias profile: learningProgressModel.profile

    title: i18n("Lesson training statistics")
    modal: true

    margins: {
        left: 40
        bottom: 40
        right: 40
        top: 40
    }

    width: parent.width - leftMargin - rightMargin
    height: parent.height - topMargin - bottomMargin
    padding: 0


    contentItem: Rectangle {
        color: colorScheme.normalBackground

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: titleLabel.font.pixelSize
            spacing: titleLabel.font.pixelSize

            Label {
                id: titleLabel
                text: lesson? lesson.title: ""
                font.bold: true
            }

            LearningProgressChart {
                id: learningProgressChart
                Layout.fillHeight: true
                Layout.fillWidth: true
                backgroundColor: colorScheme.normalBackground
                model: LearningProgressModel {
                    id: learningProgressModel
                }
            }

            Row {
                spacing: 2 * titleLabel.font.pixelSize
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Charts.LegendItem {
                    id: accuracyLegend
                    dimension: learningProgressChart.accuracy
                }
                Charts.LegendItem {
                    id: charactersPerMinuteLegend
                    dimension: learningProgressChart.charactersPerMinute
                }
            }
        }
        KColorScheme {
            id: colorScheme
            colorGroup: KColorScheme.Active
            colorSet: KColorScheme.View
        }
    }
}
