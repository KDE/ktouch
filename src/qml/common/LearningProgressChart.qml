/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Layouts 1.3
import org.kde.charts 0.1 as Charts
import ktouch 1.0

Charts.LineChart {
    id: root

    property Charts.Dimension accuracy: accuracyDimension
    property Charts.Dimension charactersPerMinute: charactersPerMinuteDimension

    pitch: 3 * Units.gridUnit

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
        Charts.Dimension {
            id: accuracyDimension
            dataColumn: 5
            color: "#ffb12d"
            minimumValue: root.minAccuracy(model.minAccuracy)
            maximumValue: 1.0
            label: i18n("Accuracy")
            unit: "%"
            unitFactor: 100
        },
        Charts.Dimension {
            id: charactersPerMinuteDimension
            dataColumn: 6
            color: "#38aef4"
            maximumValue: Math.max(Math.ceil(model.maxCharactersTypedPerMinute / 120) * 120, 120)
            label: i18n("Characters per Minute")
        }
    ]

    onElemEntered: {
        learningProgressPointTooltip.visualParent = elem
        learningProgressPointTooltip.row = row
        learningProgressPointTooltip.open()
    }

    onElemExited: {
        learningProgressPointTooltip.close()
    }

    Balloon {
        id: learningProgressPointTooltip
        property int row: -1

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
            property list<InfoItem> infoModel: [
                InfoItem {
                    title: i18nc("Statistics on lesson:", "Lesson:")
                    text: learningProgressPointTooltip.row !== -1? learningProgressPointTooltip.findLessonTitle(learningProgressModel.lessonId(learningProgressPointTooltip.row)): ""
                },
                InfoItem {
                    title: i18n("Training on:")
                    text: learningProgressPointTooltip.row !== -1?
                              learningProgressModel.date(learningProgressPointTooltip.row).toLocaleString():
                              ""
                },
                InfoItem {
                    title: i18n("Accuracy:")
                    text: learningProgressPointTooltip.row !== -1? strFormatter.formatAccuracy(learningProgressModel.accuracy(learningProgressPointTooltip.row)): ""
                },
                InfoItem {
                    title: i18n("Characters per Minute:")
                    text: learningProgressPointTooltip.row !== -1? learningProgressModel.charactersPerMinute(learningProgressPointTooltip.row): ""
                }
            ]
            width: 25 * Units.gridUnit
            model: infoModel
        }
    }

}
