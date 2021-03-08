/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *  SPDX-FileCopyrightText: 2015 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import ktouch 1.0

Item {
    property alias model: repeator.model
    id: root
    height: childrenRect.height

    KColorScheme {
        id: colorScheme
        colorGroup: KColorScheme.Active
        colorSet: KColorScheme.View
    }


    Column {
        id: column
        spacing: Units.largeSpacing
        width: parent.width
        height: childrenRect.height

        Repeater {
            id: repeator

            Row {
                spacing: Units.smallSpacing
                height: Math.max(titleLabel.height, valueLabel.height)
                width: root.width

                Label {
                    id: titleLabel
                    width: Math.round((parent.width - parent.spacing) / 2)
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignTop
                    color: colorScheme.normalText
                    opacity: 0.7
                    text: model.modelData.title
                    wrapMode: Text.Wrap
                    height: Math.max(paintedHeight, valueLabel.paintedHeight)
                }
                Label {
                    id: valueLabel
                    width: parent.width - titleLabel.width - parent.spacing
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignBottom
                    text: model.modelData.text
                    wrapMode: Text.Wrap
                    height: Math.max(paintedHeight, titleLabel.paintedHeight)
                    color: colorScheme.normalText
                }
            }
        }
    }
}
