/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *  SPDX-FileCopyrightText: 2015 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts
import ktouch 1.0
import org.kde.kirigami as Kirigami

ColumnLayout {
    id: root

    property alias model: repeator.model

    spacing: Units.largeSpacing

    Repeater {
        id: repeator

        Row {
            spacing: Units.smallSpacing
            Layout.fillWidth: true

            Label {
                id: titleLabel
                width: root.width / 2
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignTop
                color: Kirigami.Theme.textColor
                opacity: 0.7
                text: model.modelData.title
                wrapMode: Text.Wrap
            }
            Label {
                id: valueLabel
                width: root.width / 2 - Units.smallSpacing
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignBottom
                text: model.modelData.text
                wrapMode: Text.Wrap
                color: Kirigami.Theme.textColor
            }
        }
    }
}
