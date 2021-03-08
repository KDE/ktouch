/*
 *  SPDX-FileCopyrightText: 2018 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
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

