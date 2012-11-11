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
import org.kde.plasma.components 0.1 as PlasmaComponents

FocusScope {
    id: trainer

    property Lesson lesson
    property KeyboardLayout keyboardLayout
    property TrainingStats trainingStats
    property variant lines: [];

    property alias nextChar: trainingLine.nextCharacter
    property alias isCorrect: trainingLine.isCorrect
    property int position: -1
    signal finished
    signal keyPressed(variant event)
    signal keyReleased(variant event)

    function reset() {
        stats.reset()
        trainer.position = -1
        trainer.position = 0
        sheetFlick.scrollToTrainingLine()
        trainingLine.active = true
    }

    onLessonChanged: {
        trainer.lines = trainer.lesson? lesson.text.split("\n"): []
        trainer.position = -1
    }

    Component.onCompleted: {
        trainingLine.forceActiveFocus()
    }

    Flickable
    {
        id: sheetFlick
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: sheet.height + 60
        clip: true
        flickableDirection: Flickable.VerticalFlick

        function targetScrollPosition() {
            var position = trainingLine.y + sheet.y + (trainingLine.height / 2)
            return Math.max(Math.min((position - (height / 2)), contentHeight - height), 0)
        }

        function scrollToTrainingLine() {
            scrollAnimation.to = targetScrollPosition()
            scrollAnimation.start()
        }

        onHeightChanged: {
            contentY = targetScrollPosition()
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
            anchors.centerIn: parent
            width: parent.width - 60
            height: sheetContent.height + 2 * sheet.margin

            property int margin: Math.floor(0.07 * width)

            border {
                width: 1
                color: "#000"
            }

            LessonFontSizeCalculater {
                id: fontSizeCalculater
                targetWidth: sheet.width - 2 * sheet.margin
                lesson: trainer.lesson
            }

            Column {
                id: sheetContent
                anchors.centerIn: parent
                width: fontSizeCalculater.targetWidth
                height: childrenRect.height

                Text {
                    id: titleText
                    width: parent.width
                    font.pixelSize: Math.round(LessonFontSizeCalculater.BasePixelSize * 1.7 * fontSizeCalculater.scale)
                    wrapMode: Text.Wrap
                    lineHeight: 1.5
                    text: trainer.lesson? lesson.title: ""
                }
                Item {
                    height: Math.round(LessonFontSizeCalculater.BasePixelSize * fontSizeCalculater.scale)
                    width: 1
                }
                Repeater {
                    id: lines
                    model: trainer.lines.length
                    Item {
                        property bool isDone: trainer.position > index
                        width: sheetContent.width
                        height: Math.ceil (1.5 * text.height * text.scale)
                        Text {
                            id: text
                            color: isDone? "#000": "#888"
                            text: trainer.lines[index]
                            textFormat: Text.PlainText
                            font.family: "monospace"
                            font.pixelSize: LessonFontSizeCalculater.BasePixelSize
                            opacity: trainer.position == index? 0: 1
                            scale: fontSizeCalculater.scale
                            transformOrigin: Item.TopLeft
                            smooth: true
                        }
                    }
                    onModelChanged: trainer.position = 0
                }
            }

            TrainingLine {
                id: trainingLine
                fontScale: fontSizeCalculater.scale
                charWidth: fontSizeCalculater.charWidth
                property Item target: lines.itemAt(trainer.position)
                onDone: {
                    if (trainer.position < trainer.lines.length - 1)
                    {
                        trainer.position++
                        sheetFlick.scrollToTrainingLine()
                    }
                    else
                    {
                        trainingLine.active = false
                        trainer.finished();
                        stats.stopTraining();
                    }
                }
                onKeyPressed: trainer.keyPressed(event)
                onKeyReleased: trainer.keyReleased(event)
                y: target? target.y + sheet.margin: 0
                x: sheet.margin
                width: target? target.width: 0
                referenceLine: trainer.position >= 0 && trainer.position < trainer.lines.length?
                    trainer.lines[trainer.position]: ""
                trainingStats: trainer.trainingStats

                KeyItem {
                    id: hintKey
                    anchors {
                        horizontalCenter: trainingLine.horizontalCenter
                        top: trainingLine.bottom
                        topMargin: LessonFontSizeCalculater.BasePixelSize
                    }
                    property real scaleFactor: 1
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
        }
        MouseArea {
            anchors.fill: parent
            onClicked: trainingLine.forceActiveFocus()
        }
    }

    PlasmaComponents.ScrollBar {
        flickableItem: sheetFlick
    }
}

