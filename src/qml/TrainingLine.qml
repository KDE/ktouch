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
import ktouch 1.0

TrainingLineCore {
    id: line
    property real fontScale
    property real charWidth

    signal keyPressed(variant event)
    signal keyReleased(variant event)

    cursorItem: cursor

    height: line.fontScale * LessonFontSizeCalculater.BasePixelSize
    focus: true

    onFocusChanged: {
        if (!line.activeFocus) {
            trainingStats.stopTraining()
        }
    }

    Keys.onPressed: {
        if (!line.active)
            return

        cursorAnimation.restart()
        trainingStats.startTraining()
        stopTimer.restart()

        if (!event.isAutoRepeat) {
            line.keyPressed(event)
        }
    }

    Keys.onReleased: {
        if (!line.active)
            return

        if (!event.isAutoRepeat) {
            line.keyReleased(event)
        }
    }

    Timer {
        id: stopTimer
        interval: 5000
        onTriggered: {
            stats.stopTraining()
        }
    }

    Repeater {
        id: lineChars
        model: referenceLine.length

        Rectangle {
            id: lineCharWrapper
            property alias text: lineChar.text
            transformOrigin: Item.Center
            x: index * line.charWidth
            y: 0
            width: lineChar.width * line.fontScale
            height: lineChar.height * line.fontScale
            state: {
                var actualLength = line.actualLine.length

                if (index < actualLength)
                {
                    var charCorrect = line.actualLine[index] === line.referenceLine[index]
                    var previousCorrect = index == 0 || lineChars.itemAt(index - 1).state === "done"
                    return charCorrect && previousCorrect? "done": "error"
                }

                if (index - actualLength < line.preeditString.length)
                {
                    return "preedit"
                }

                return "placeholder"
            }

            onStateChanged: {
                if (state == "error")
                    errorAnimation.restart()
            }

            Text {
                id: lineChar
                font.family: "monospace"
                font.pixelSize: LessonFontSizeCalculater.BasePixelSize
                text: {
                    var actualLength = line.actualLine.length
                    var character = index < actualLength?
                                line.actualLine[index]:
                                index - actualLength < line.preeditString.length?
                                    line.preeditString[index - actualLength]:
                                    line.referenceLine[index]

                    if (character === " " && lineCharWrapper.state === "error")
                        return "\u2423"

                    return character
                }
                textFormat: Text.PlainText
                smooth: true
                transformOrigin: Item.TopLeft
                scale: line.fontScale

                SequentialAnimation {
                    id: errorAnimation
                    NumberAnimation {
                        target: lineCharWrapper
                        property: "scale"
                        to: 1.3
                        duration: 100
                        easing.type: Easing.OutBack
                        easing.overshoot: 20
                    }
                    NumberAnimation {
                        target: lineCharWrapper
                        property: "scale"
                        to: 1.0
                        duration: 50
                        easing.type: Easing.InCubic
                    }
                }
            }

            states: [
                State {
                    name: "placeholder"
                    PropertyChanges {
                        target: lineCharWrapper
                        color: "#00000000"

                    }
                    PropertyChanges {
                        target: lineChar
                        color: "#888"
                    }
                },
                State {
                    name: "preedit"
                    PropertyChanges {
                        target: lineCharWrapper
                        color: "#d0d0d0"

                    }
                    PropertyChanges {
                        target: lineChar
                        color: "#000"
                    }
                },
                State {
                    name: "done"
                    PropertyChanges {
                        target: lineCharWrapper
                        color: "#00000000"

                    }
                    PropertyChanges {
                        target: lineChar
                        color: "#000"
                    }
                },
                State {
                    name: "error"
                    PropertyChanges {
                        target: lineCharWrapper
                        color: "#00000000"

                    }
                    PropertyChanges {
                        target: lineChar
                        color: "#f00"
                    }
                }
            ]
        }
    }

    Rectangle {
        id: cursor
        property Item target: lineChars.itemAt(line.actualLine.length - 1)
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: Math.round(target? target.x + target.width: 0)
        }
        width: 1
        height: Math.round(1.2 * line.fontScale * LessonFontSizeCalculater.BasePixelSize)
        color: "#000"
        visible: line.active && line.activeFocus
        SequentialAnimation {
            id: cursorAnimation
            running: line.active && line.activeFocus && Qt.application.active
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
