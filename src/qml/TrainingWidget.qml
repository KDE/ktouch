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

import QtQuick 1.0
import ktouch 1.0
import org.kde.plasma.components 0.1 as PlasmaComponents

FocusScope {
    id: trainer

    property Lesson lesson
    property variant lines: [];

    property string nextChar
    property bool isCorrect: trainingLine.isCorrect
    property int fontSize: 20
    property int position: -1
    property int lineHeight: 2 * fontSize
    property int margin: 30
    signal finished
    signal keyPressed(variant event)
    signal keyReleased(variant event)

    function reset() {
        stats.reset()
        trainer.position = -1
        trainer.position = 0
        sheetFlick.scrollToTrainingLine()
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

        function targetScrollPosition()
        {
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

        NumberAnimation
        {
            target: sheetFlick
            id: scrollAnimation
            duration: 150
            property: "contentY"
        }

        Rectangle
        {
            id: sheet
            color: "#fff"
            anchors.centerIn: parent
            width: childrenRect.width
            height: childrenRect.height

            border {
                width: 1
                color: "#000"
            }

            Column {
                width: childrenRect.width
                height: childrenRect.height
                Item {
                    height: margin
                    width: 1
                }
                Item {
                    height: fontSize * 2
                    width: titleText.width + 2 * margin
                    Text {
                        id: titleText
                        anchors.centerIn: parent
                        font.pixelSize: fontSize * 1.7
                        text: trainer.lesson? lesson.title: ""
                    }
                }
                Item {
                    height: 15
                    width: 1
                }
                Repeater {
                    id: lines
                    model: trainer.lines.length
                    Item {
                        property bool isDone: trainer.position > index
                        width: text.width + 2 * margin
                        height: lineHeight
                        Text {
                            id: text
                            anchors.centerIn: parent
                            color: isDone? "#000": "#888"
                            text: trainer.lines[index]
                            textFormat: Text.PlainText
                            font.family: "monospace"
                            font.pixelSize: fontSize
                            opacity: trainer.position == index? 0: 1
                        }
                    }
                    onModelChanged: trainer.position = 0
                }
                Item {
                    height: margin
                    width: 1
                }
            }

            TrainingLine {
                id: trainingLine
                property Item target: lines.itemAt(trainer.position)
                onDone: {
                    if (trainer.position < trainer.lines.length - 1)
                    {
                        trainer.position++
                        sheetFlick.scrollToTrainingLine()
                    }
                    else
                    {
                        trainer.finished();
                        stats.stopTraining();
                    }
                }
                onKeyPressed: trainer.keyPressed(event)
                onKeyReleased: trainer.keyReleased(event)
                onNewNextChar: trainer.nextChar = nextChar
                y: target? target.y: 0
                x: 0
                width: target? target.width: 0
                height: lineHeight
                text: trainer.position >= 0 && trainer.position < trainer.lines.length?
                    trainer.lines[trainer.position]: ""
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

