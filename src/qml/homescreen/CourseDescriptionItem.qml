/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *  SPDX-FileCopyrightText: 2015 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import ktouch 1.0

import "../common"

Collapsable {
    id: root

    property string description

    onDescriptionChanged: {
        if (contentItem.opacity === 0) {
            descriptionLabel.text = description
        }
        else {
            swapContent()
        }
    }

    onContentReadyForSwap: {
        descriptionLabel.text = description
    }

    Label {
        id: descriptionLabel
        anchors {
            fill: parent
            leftMargin: Units.gridUnit
            rightMargin: Units.gridUnit
            topMargin: Units.largeSpacing
            bottomMargin: Units.largeSpacing
        }

        wrapMode: Text.Wrap
    }

}
