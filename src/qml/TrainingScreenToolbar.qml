/*
 *  Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
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

import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

Item {
    id: item

    property bool trainingStarted: false
    property bool trainingFinished: true
    property Item menuOverlayItem

    function setMessage() {
        if (!stats.timeIsRunning && !trainingFinished) {
            messageBox.showMessage(
                        trainingStarted?
                            i18n("Training session started. Time begins running with the first key stroke."):
                            i18n("Training session interrupted. Time begins running again with next key stroke."))
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

    PlasmaCore.FrameSvgItem {
        anchors.fill: parent
        imagePath: findImage("trainingscreen.svgz")
        prefix: "toolbar"
    }

    Row {
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: parent.right
            leftMargin: 3
            rightMargin: 3
            verticalCenterOffset: -1
        }

        spacing: 3
        height: pauseButton.height

        PlasmaComponents.ToolButton {
            id: menuButton
            iconSource: "go-home"
            onClicked: item.menuOverlayItem.show()
        }

        PlasmaComponents.ToolButton {
            id: pauseButton
            iconSource: "media-playback-pause"
            checked: !stats.timeIsRunning
            onClicked: stats.stopTraining()
            onCheckedChanged: setMessage()
        }

        MessageBox {
            id: messageBox
        }
    }
}
