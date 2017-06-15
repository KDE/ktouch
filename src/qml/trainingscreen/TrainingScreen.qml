/*
 *  Copyright 2013  Sebastian Gottfried <sebastiangottfried@web.de>
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

import QtQuick 2.5
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import ktouch 1.0

import "../keyboard"
import "../meters"

FocusScope {

    id: screen

    property KeyboardLayout keyboardLayout
    property Profile profile
    property Course course
    property Lesson lesson

    property alias stats: stats
    property alias referenceStats: referenceStats

    signal restartRequested()
    signal abortRequested()
    signal finished()

    property bool trainingStarted: false
    property bool trainingFinished: true
    property bool isActive: Qt.application.active

    function setLessonKeys() {
        if (!lesson)
            return;

        var chars = lesson.characters;
        var keyItems = keyboard.keyItems()
        var modifierItems = []
        var usedModifiers = {}

        for (var i = 0; i < keyItems.length; i++) {
            var key = keyItems[i].key
            if (key.keyType() == "key") {
                keyItems[i].enabled = false;
                for (var j = 0; j < key.keyCharCount; j++) {
                    var keyChar = key.keyChar(j)
                    if (chars.indexOf(keyChar.value) != -1) {
                        keyItems[i].enabled = true;
                        if (keyChar.modifier !== "") {
                            usedModifiers[keyChar.modifier] = true
                        }
                    }
                }
            }
            else {
                var type = keyItems[i].key.type
                if (type != SpecialKey.Return && type != SpecialKey.Backspace && type != SpecialKey.Space)
                {
                    modifierItems.push(keyItems[i])
                    keyItems[i].enabled = false
                }
                else if (type == SpecialKey.Return && preferences.nextLineWithSpace) {
                    keyItems[i].enabled = false
                }
            }
        }

        for (i = 0; i < modifierItems.length; i++) {
            var modifierItem = modifierItems[i]
            modifierItem.enabled = !!usedModifiers[modifierItem.key.modifierId]
        }
    }

    function reset() {
        toolbar.reset()
        trainingWidget.reset()
        screen.trainingStarted = false
        screen.trainingFinished = true
        profileDataAccess.loadReferenceTrainingStats(referenceStats, screen.profile, screen.course.id, screen.lesson.id)
        profileDataAccess.saveCourseProgress(lesson.id, profile, course.id, ProfileDataAccess.LastSelectedLesson)
    }

    function start() {
        screen.trainingFinished = false
        screen.trainingStarted = true
    }

    function forceActiveFocus() {
        trainingWidget.forceActiveFocus()
    }

    onLessonChanged: setLessonKeys()

    onIsActiveChanged: {
        if (!screen.isActive) {
            stats.stopTraining()
        }
    }

    TrainingStats {
        id: stats
        onTimeIsRunningChanged: {
            if (timeIsRunning) {
                screen.trainingStarted = false
            }
        }
    }

    TrainingStats {
        id: referenceStats
    }

    Shortcut {
        sequence: "Escape"
        enabled: screen.visible
        onActivated: {
            if (menuOverlay.active) {
                menuOverlay.hide()
            }
            else {
                menuOverlay.show()
            }
        }

    }


    ColumnLayout {
        id: screenContent
        anchors.fill: parent
        spacing: 0
        BorderImage {
            Layout.fillWidth: true
            Layout.preferredHeight: 41
            border {
                top: 1
                bottom: 1
            }
            cache: false
            source: utils.findImage("trainingscreen-toolbar.png")
            horizontalTileMode: BorderImage.Repeat
            verticalTileMode: BorderImage.Repeat

            TrainingScreenToolbar {
                id: toolbar
                anchors.fill: parent
                trainingStarted: screen.trainingStarted
                trainingFinished: screen.trainingFinished
                stats: stats
                menuOverlayItem: menuOverlay
            }
        }

        BorderImage {
            id: header
            Layout.fillWidth: true
            Layout.preferredHeight: visible? 130: 0
            visible: preferences.showStatistics
            border {
                top: 1
                bottom: 1
            }
            source: utils.findImage("trainingscreen-header.png")
            cache: false

            StatBox {
                anchors.centerIn: parent
                width: parent.width - 60
                stats: stats
                referenceStats: referenceStats
            }
        }


        BorderImage {
            id: body
            Layout.fillWidth: true
            Layout.fillHeight: true
            border {
                top: 1
                bottom: 1
            }
            source: utils.findImage("trainingscreen-viewport.png")
            cache: false

            TrainingWidget {
                id: trainingWidget
                anchors.fill: parent
                lesson: screen.lesson
                keyboardLayout: screen.keyboardLayout
                trainingStats: stats
                overlayContainer: trainingOverlayContainer
                onKeyPressed: keyboard.handleKeyPress(event)
                onKeyReleased: keyboard.handleKeyRelease(event)
                onNextCharChanged: keyboard.updateKeyHighlighting()
                onIsCorrectChanged: keyboard.updateKeyHighlighting()
                onFinished: {
                    profileDataAccess.saveTrainingStats(stats, screen.profile, screen.course.id, screen.lesson.id)
                    screen.finished(stats)
                    screen.trainingFinished = true
                }
            }

            BorderImage {
                anchors.fill: parent
                border {
                    top: 3
                    bottom: 3
                }
                source: utils.findImage("trainingscreen-viewport-shadow.png")
                cache: false

            }

        }

        BorderImage {
            id: footer
            visible: preferences.showKeyboard
            Layout.fillWidth: true
            Layout.preferredHeight: visible?
                        screen.keyboardLayout.isValid?
                            Math.round(Math.min((parent.height - toolbar.height - header.height) / 2, parent.width / keyboard.aspectRatio)):
                            keyboardUnavailableNotice.height:
                        0
            border {
                top: 1
                bottom: 1
            }
            source: utils.findImage("trainingscreen-footer.png")
            cache: false
            Keyboard {
                id: keyboard

                property variant highlightedKeys: []
                function highlightKey(which) {
                    for (var i = 0; i < highlightedKeys.length; i++)
                        highlightedKeys[i].isHighlighted = false
                    var keys = findKeyItems(which)
                    var newHighlightedKeys = []
                    for (var index = 0; index < keys.length ; index++) {
                        var key = keys[index]
                        if (key) {
                            key.isHighlighted = true
                            newHighlightedKeys.push(key)
                            if (typeof which == "string") {
                                for (var i = 0; i < key.key.keyCharCount; i++) {
                                    var keyChar = key.key.keyChar(i)
                                    if (keyChar.value == which && keyChar.modifier != "") {
                                        var modifier = findModifierKeyItem(keyChar.modifier)
                                        if (modifier) {
                                            modifier.isHighlighted = true
                                            newHighlightedKeys.push(modifier)
                                        }
                                        break
                                    }
                                }
                            }
                        }
                    }
                    highlightedKeys = newHighlightedKeys
                }

                function updateKeyHighlighting() {
                    if (!visible)
                        return;
                    if (trainingWidget.isCorrect) {
                        if (trainingWidget.nextChar !== "") {
                            highlightKey(trainingWidget.nextChar)
                        }
                        else {
                            highlightKey(preferences.nextLineWithSpace? Qt.Key_Space: Qt.Key_Return)
                        }
                    }
                    else {
                        highlightKey(Qt.Key_Backspace)
                    }
                }

                keyboardLayout: screen.keyboardLayout
                anchors {
                    fill: parent
                    leftMargin: 30
                    rightMargin: 30
                    topMargin: 10
                    bottomMargin: 10
                }

                onKeyboardUpdate: {
                    setLessonKeys()
                    highlightedKeys = []
                    updateKeyHighlighting()
                }
            }

            KeyboardUnavailableNotice {
                id: keyboardUnavailableNotice
                visible: !screen.keyboardLayout.isValid
            }
        }
    }

    Item {
        id: trainingOverlayContainer
        anchors.fill: parent
    }

    TrainingScreenMenuOverlay {
        id: menuOverlay
        blurSource: screenContent
        anchors.fill: parent
        onClosed: trainingWidget.forceActiveFocus()
        onRestartRequested: screen.restartRequested()
        onAbortRequested: screen.abortRequested()
    }
}
