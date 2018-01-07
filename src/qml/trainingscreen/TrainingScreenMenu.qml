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

import '../common'


PopupDialog {
    id: root

    signal restartRequested()
    signal abortRequested()

    onOpened: {
        resumeButton.forceActiveFocus()
    }

    title: i18n("Menu")

    padding: 20

    contentItem: Item {
        width: column.width
        height: column.height

        Column {
            id: column
            focus: true
            anchors.centerIn: parent
            spacing: 20
            width: 1.5 * Math.max(resumeButton.implicitWidth, restartButton.implicitWidth, returnButton.implicitWidth)

            IconButton {
                id: resumeButton
                iconName: "go-next-view"
                text: i18n("Resume Training")
                bgColor: colorScheme.positiveBackground
                width: parent.width
                onClicked: close()
                KeyNavigation.backtab: returnButton
                KeyNavigation.tab: restartButton
                KeyNavigation.down: restartButton
            }

            IconButton {
                id: restartButton
                iconName: "view-refresh"
                text: i18n("Restart Lesson")
                width: parent.width
                onClicked: {
                    close()
                    restartRequested()
                }
                KeyNavigation.backtab: resumeButton
                KeyNavigation.tab: returnButton
                KeyNavigation.up: resumeButton
                KeyNavigation.down: returnButton
            }

            IconButton {
                id: returnButton
                iconName: "go-home"
                text: i18n("Return to Home Screen")
                width: parent.width
                onClicked: {
                    close()
                    abortRequested()
                }
                KeyNavigation.backtab: restartButton
                KeyNavigation.tab: resumeButton
                KeyNavigation.up: restartButton
            }
        }
    }
}

