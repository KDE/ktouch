/*
 *  SPDX-FileCopyrightText: 2018 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import ktouch 1.0

import "../keyboard"
import "../common"

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
            contentHeight: sheet.height + 2 * Units.gridUnit
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
                duration: Units.shortDuration
                property: "contentY"
            }

            Rectangle {
                id: sheet
                color: "#fff"
                x: Units.gridUnit
                y: Units.gridUnit
                width: trainingWidget.width - 2 * Units.gridUnit
                height: lessonPainter.height

                border {
                    width: Units.borderWidth
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
            NumberAnimation { duration: Units.shortDuration }
        }
    }
}

