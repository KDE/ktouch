/*
 *  Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *  Copyright 2015  Sebastian Gottfried <sebastiangottfried@web.de>
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

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import ktouch 1.0

import "../common"

Item {
    id: item

    property TrainingStats stats
    property bool trainingStarted: false
    property bool trainingFinished: true
    property Item menuOverlayItem

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
            verticalCenterOffset: -1
        }

        spacing: 3
        height: menuButton.height

        ToolButton {
            id: menuButton
            iconName: "go-home"
            onClicked: item.menuOverlayItem.show()
        }

        MessageBox {
            id: messageBox
        }
    }
}
