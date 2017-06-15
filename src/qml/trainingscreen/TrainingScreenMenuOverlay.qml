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
import QtGraphicalEffects 1.0
import ktouch 1.0

Loader {
    id: loader

    property Item blurSource;

    signal closed()
    signal restartRequested()
    signal abortRequested()

    active: false

    function show() {
        loader.active = true
    }

    function hide() {
        item.opacity = 0
    }

    sourceComponent:  Component {
        FocusScope {
            id: item
            anchors.fill: parent

            opacity: loader.status == Loader.Ready? 1: 0

            Component.onCompleted: resumeButton.forceActiveFocus()

            Behavior on opacity {
                SequentialAnimation {
                    NumberAnimation {
                        duration: 300
                        easing.type: Easing.InOutQuad
                    }
                    ScriptAction {
                        script: {
                            if (opacity == 0) {
                                loader.active = false;
                                closed()
                            }
                        }
                    }
                }
            }


            ShaderEffectSource {
                id: effectSource
                sourceItem: blurSource
                anchors.fill: parent
                hideSource: false
            }

            HueSaturation {
                id: desaturatedBackground
                source: effectSource
                anchors.fill: parent
                lightness: -0.3
                saturation: -0.5
                visible: false
            }

            FastBlur {
                anchors.fill: parent
                source: desaturatedBackground
                radius: 50
            }

            Rectangle {
                anchors.fill: parent
                color: "#55000000"
            }

            /* swallow all mouse events */
            MouseArea {
                anchors.fill: parent
                enabled: false
                hoverEnabled: enabled
            }


            GroupBox {
                id: groupBox
                anchors.centerIn: parent
                width: column.width + 30
                height: column.height + 30

                Column {
                    id: column
                    focus: true
                    anchors.centerIn: parent
                    spacing: 15
                    width: Math.max(resumeButton.implicitWidth, restartButton.implicitWidth, returnButton.implicitWidth)

                    Button {
                        id: resumeButton
                        iconName: "go-next-view"
                        text: i18n("Resume Training")
                        width: parent.width
                        onClicked: hide()
                        KeyNavigation.backtab: returnButton
                        KeyNavigation.tab: restartButton
                        KeyNavigation.down: restartButton
                    }

                    Button {
                        id: restartButton
                        iconName: "view-refresh"
                        text: i18n("Restart Lesson")
                        width: parent.width
                        onClicked: {
                            restartRequested()
                            hide()
                        }
                        KeyNavigation.backtab: resumeButton
                        KeyNavigation.tab: returnButton
                        KeyNavigation.up: resumeButton
                        KeyNavigation.down: returnButton
                    }

                    Button {
                        id: returnButton
                        iconName: "go-home"
                        text: i18n("Return to Home Screen")
                        width: parent.width
                        onClicked: {
                            abortRequested()
                            hide()
                        }
                        KeyNavigation.backtab: restartButton
                        KeyNavigation.tab: resumeButton
                        KeyNavigation.up: restartButton
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
        }

    }
}

