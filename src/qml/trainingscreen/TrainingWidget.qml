/*
 *  Copyright 2013  Sebastian Gottfried <sebastiangottfried@web.de>
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
import ktouch 1.0

import "../keyboard"

FocusScope {
    id: trainingWidget

    property Lesson lesson
    property KeyboardLayout keyboardLayout
    property TrainingStats trainingStats
    property Item overlayContainer

    property alias nextChar: trainingLine.nextCharacter
    property alias isCorrect: trainingLine.isCorrect
    property int position: -1
    signal finished
    signal keyPressed(variant event)
    signal keyReleased(variant event)

    function reset() {
        stats.reset()
        lessonPainter.reset()
        sheetFlick.scrollToCurrentLine()
        trainingLine.active = true
    }

    function forceActiveFocus() {
        trainingLine.forceActiveFocus()
    }

    Timer {
        id: stopTimer
        interval: 5000
        onTriggered: {
            stats.stopTraining()
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: trainingLine.forceActiveFocus()
    }

    ScrollView {
        anchors.fill: parent
        Flickable
        {
            id: sheetFlick
            anchors.fill: parent
            contentWidth: parent.width
            contentHeight: sheet.height + 60
            clip: true
            flickableDirection: Flickable.VerticalFlick

            function currentLineY() {
                var cursorRect = lessonPainter.cursorRectangle
                var y = cursorRect.y + sheet.y + (cursorRect.height / 2)
                return Math.max(Math.min((y - (height / 2)), contentHeight - height), 0)
            }

            function scrollToCurrentLine() {
                scrollAnimation.to = currentLineY()
                scrollAnimation.start()
            }

            onHeightChanged: {
                contentY = currentLineY()
            }

            NumberAnimation {
                target: sheetFlick
                id: scrollAnimation
                duration: 150
                property: "contentY"
            }

            Rectangle {
                id: sheet
                color: "#fff"
                x: 30
                y: 30
                width: trainingWidget.width - 60
                height: lessonPainter.height

                border {
                    width: 1
                    color: "#000"
                }

                LessonPainter {
                    id: lessonPainter
                    anchors.centerIn: sheet
                    lesson: trainingWidget.lesson
                    maximumWidth: parent.width
                    trainingLineCore: trainingLine

                    onDone: {
                        trainingLine.active = false
                        trainingWidget.finished();
                        stats.stopTraining();
                    }

                    TrainingLineCore {
                        id: trainingLine
                        anchors.fill: parent
                        focus: true
                        trainingStats: stats
                        cursorItem: cursor

                        onActiveFocusChanged: {
                            if (!trainingLine.activeFocus) {
                                trainingStats.stopTraining()
                            }
                        }

                        Keys.onPressed: {
                            if (!trainingLine.active)
                                return

                            cursorAnimation.restart()
                            trainingStats.startTraining()
                            stopTimer.restart()

                            if (!event.isAutoRepeat) {
                                trainingWidget.keyPressed(event)
                            }
                        }

                        Keys.onReleased: {
                            if (!trainingLine.active)
                                return

                            if (!event.isAutoRepeat) {
                                trainingWidget.keyReleased(event)
                            }
                        }

                        KeyNavigation.backtab: trainingLine
                        KeyNavigation.tab: trainingLine
                    }

                    Rectangle {
                        id: cursor
                        color: "#000"
                        x: Math.floor(lessonPainter.cursorRectangle.x)
                        y: lessonPainter.cursorRectangle.y
                        width: lessonPainter.cursorRectangle.width
                        height: lessonPainter.cursorRectangle.height

                        onYChanged: sheetFlick.scrollToCurrentLine()

                        SequentialAnimation {
                            id: cursorAnimation
                            running: trainingLine.active && trainingLine.activeFocus && Qt.application.active
                            loops: Animation.Infinite
                            PropertyAction {
                                target: cursor
                                property: "opacity"
                                value: 1
                            }
                            PauseAnimation {
                                duration: 500
                            }
                            PropertyAction {
                                target: cursor
                                property: "opacity"
                                value: 0
                            }
                            PauseAnimation {
                                duration: 500
                            }
                        }
                    }
                }
            }
        }
    }

    KeyItem {
        id: hintKey
        parent: trainingWidget.overlayContainer
        anchors.horizontalCenter: parent.horizontalCenter
        y: Math.max(height, Math.min(parent.height - 2 * height,
            sheetFlick.mapToItem(parent, 0, cursor.y + 3 * cursor.height - sheetFlick.contentY).y))

        property real horizontalScaleFactor: 1
        property real verticalScaleFactor: 1
        property Key defaultKey: Key {}
        property KeyboardLayout defaultKeyboardLayout: KeyboardLayout {}

        key: {
            var specialKeyType

            switch (trainingLine.hintKey) {
            case Qt.Key_Return:
                specialKeyType = SpecialKey.Return
                break
            case Qt.Key_Backspace:
                specialKeyType =  SpecialKey.Backspace
                break
            case Qt.Key_Space:
                specialKeyType =  SpecialKey.Space
                break
            default:
                specialKeyType =  -1
            }

            for (var i = 0; i < keyboardLayout.keyCount; i++) {
                var key = keyboardLayout.key(i)

                if (key.keyType() === "specialKey" && key.type === specialKeyType) {
                    return key;
                }
            }

            return defaultKey
        }

        opacity: trainingLine.hintKey !== -1? 1: 0
        isHighlighted: opacity == 1
        keyboardLayout: screen.keyboardLayout || defaultKeyboardLayout
        Behavior on opacity {
            NumberAnimation { duration: 150 }
        }
    }
}

