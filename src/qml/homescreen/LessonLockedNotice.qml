/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *  SPDX-FileCopyrightText: 2015 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import ktouch 1.0

import '../common'

Item {
    id: root

    property color glowColor: "#ffffff"

    KColorScheme {
        id: palette
        colorGroup: KColorScheme.Active
        colorSet: KColorScheme.Window
    }

    ColumnLayout {
        id: content

        anchors.centerIn: parent
        width: parent.witdh
        spacing: Units.largeSpacing

        Icon {
            id: icon
            icon: "object-locked"
            Layout.preferredWidth: Units.fontMetrics.roundedIconSize(8 * Units.gridUnit)
            Layout.preferredHeight: Layout.preferredWidth
            Layout.alignment: Qt.AlignHCenter
        }


        Label {
            id: text
            text: i18n("Complete Previous Lessons to Unlock")
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
            topPadding: Units.largeSpacing
            bottomPadding: Units.largeSpacing
            background: Rectangle {
                radius: text.font.pixelSize
                color: palette.neutralBackground
            }

            FontMetrics {
                id: metrics
                font: text.font
            }

            Layout.alignment: Qt.AlignHCenter
            Layout.maximumWidth: root.width
            Layout.preferredWidth: metrics.boundingRect(text.text).width + 2 * text.font.pixelSize
        }
    }

    Glow {
        anchors.fill: content
        source: content
        color: root.glowColor
        radius: Math.floor(.75 * Units.gridUnit)
        samples: 2 * radius + 1
    }
}
