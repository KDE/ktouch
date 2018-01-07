/*
 *  Copyright 2018  Sebastian Gottfried <sebastian.gottfried@posteo.de>
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
            leftMargin: 30
            rightMargin: 30
        }

        spacing: 5

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
