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

    signal restartRequested()
    signal abortRequested()

    visible: false
    opacity: 0

    function show() {
        focusScope.forceActiveFocus()
        fadeInAnimation.start()
    }

    function hide() {
        fadeOutAnimation.start()
    }

    Rectangle {
        anchors.fill: parent
        color: "#55000000"
    }

    /* swallow all mouse events */
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
    }

    FocusScope {
        id: focusScope
        anchors.centerIn: parent

        PlasmaCore.FrameSvgItem  {
            id: frame
            anchors.centerIn: parent
            imagePath: "widgets/frame"
            prefix: "raised"
            width: column.width + frame.margins.left + frame.margins.right + 30
            height: column.height + frame.margins.top + frame.margins.bottom + 30

        }

        Column {
            id: column
            anchors.centerIn: parent
            spacing: 15
            width: Math.max(resumeButton.implicitWidth, restartButton.implicitWidth, returnButton.implicitWidth)

            PlasmaComponents.Button {
                focus: true
                id: resumeButton
                iconSource: "go-next-view"
                text: i18n("Resume Training")
                width: parent.width
                onClicked: hide()
            }

            PlasmaComponents.Button {
                id: restartButton
                iconSource: "view-refresh"
                text: i18n("Restart Lesson")
                width: parent.width
                onClicked: {
                    restartRequested()
                    hide()
                }
            }

            PlasmaComponents.Button {
                id: returnButton
                iconSource: "go-home"
                text: i18n("Return to Home Screen")
                width: parent.width
                onClicked: {
                    abortRequested()
                    hide()
                }
            }

            Keys.onDownPressed: {
                if (resumeButton.focus)
                    restartButton.focus = true;
                else if (restartButton.focus)
                    returnButton.focus = true;
            }

            Keys.onUpPressed: {
                if (restartButton.focus)
                    resumeButton.focus = true;
                else if (returnButton.focus)
                    restartButton.focus = true;
            }

            Keys.onEscapePressed: {
                hide()
            }
        }
    }

    SequentialAnimation {
        id: fadeInAnimation
        PropertyAction {
            target: item
            property: "visible"
            value: true
        }
        NumberAnimation {
            target: item
            property: "opacity"
            to: 1
            duration: 300
            easing.type: Easing.InOutQuad
        }
    }

    SequentialAnimation {
        id: fadeOutAnimation
        NumberAnimation {
            target: item
            property: "opacity"
            to: 0
            duration: 300
            easing.type: Easing.InOutQuad
        }
        PropertyAction {
            target: item
            property: "visible"
            value: false
        }
    }
}
