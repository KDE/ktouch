/*
 *  SPDX-FileCopyrightText: 2018 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Layouts 1.3
import ktouch 1.0

import "../common"

ToolBar {
    id: root

    property TrainingStats stats
    property bool trainingStarted: false
    property bool trainingFinished: true
    property PopupDialog menu

    dimFactor: 1.8

    function setMessage() {
        if (!stats.timeIsRunning && !trainingFinished) {
            var msg = trainingStarted?
                i18n("Training session started. Time begins running with the first keystroke."):
                i18n("Training session interrupted. Time begins running again with next keystroke.")
            messageBox.showMessage(msg, "media-playback-pause")
        }
        else {
            messageBox.clearMessage()
        }
    }

    function reset() {
        messageBox.clearMessageImmediately()
    }

    onTrainingStartedChanged: setMessage()
    onTrainingFinishedChanged: setMessage()

    Connections {
        target: stats
        onTimeIsRunningChanged: setMessage()
    }

    Row {
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: parent.right
            leftMargin: Units.gridUnit
            rightMargin: Units.gridUnit
        }

        spacing: Units.largeSpacing

        IconToolButton {
            id: menuButton
            iconName: "go-home"
            onClicked: root.menu.open()
            colorScheme: root.colorScheme
        }

        MessageBox {
            id: messageBox
        }
    }
}
