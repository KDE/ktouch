/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Layouts 1.3
import ktouch 1.0
import org.kde.quickcharts as Charts
import org.kde.quickcharts.controls as ChartsControls

import '../common'

PopupDialog {
    id: root

    property alias course: learningProgressModel.courseFilter
    property alias lesson: learningProgressModel.lessonFilter
    property alias profile: learningProgressModel.profile

    title: i18n("Lesson training statistics")
    modal: true

    margins: {
        left: 2 * Units.gridUnit
        bottom: 2 * Units.gridUnit
        right: 2 * Units.gridUnit
        top: 2 * Units.gridUnit
    }

    width: parent.width - leftMargin - rightMargin
    height: parent.height - topMargin - bottomMargin
    padding: 0


    contentItem: Rectangle {
        color: colorScheme.normalBackground

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: Units.gridUnit
            spacing: Units.gridUnit

            Label {
                id: titleLabel
                text: lesson? lesson.title: ""
                font.bold: true
            }

            LearningProgressChart {
                id: learningProgressChart
                Layout.fillHeight: true
                Layout.fillWidth: true
                model: LearningProgressModel {
                    id: learningProgressModel
                }
            }

            ChartsControls.Legend {
                id: accuracyLegend
                chart: learningProgressChart
            }
        }
        KColorScheme {
            id: colorScheme
            colorGroup: KColorScheme.Active
            colorSet: KColorScheme.View
        }
    }
}
