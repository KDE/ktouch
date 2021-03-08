/*
 *  SPDX-FileCopyrightText: 2018 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Layouts 1.3
import ktouch 1.0

import "../common"
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

    KColorScheme {
        id: colorScheme
        colorGroup: KColorScheme.Active
        colorSet: KColorScheme.Complementary
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
            if (!menu.visible) {
                menu.open()
            }
        }
    }


    ColumnLayout {
        id: screenContent
        anchors.fill: parent
        spacing: 0
        BorderImage {
            Layout.fillWidth: true
            Layout.preferredHeight: toolbar.implicitHeight + 1
            border {
                top: 1
                bottom: 1
            }
            cache: false
            source: "qrc:///ktouch/images/trainingscreen-toolbar.png"
            horizontalTileMode: BorderImage.Repeat
            verticalTileMode: BorderImage.Repeat

            TrainingScreenToolbar {
                id: toolbar
                anchors.fill: parent
                trainingStarted: screen.trainingStarted
                trainingFinished: screen.trainingFinished
                stats: stats
                menu: menu
            }
        }

        BorderImage {
            id: header
            Layout.fillWidth: true
            Layout.preferredHeight: visible? statBox.height + 2 * Units.gridUnit: 0
            visible: preferences.showStatistics
            border {
                top: 1
                bottom: 1
            }
            source: "qrc:///ktouch/images/trainingscreen-header.png"
            cache: false

            StatBox {
                id: statBox
                anchors.centerIn: parent
                width: parent.width - 2 * Units.gridUnit
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
            source: "qrc:///ktouch/images/trainingscreen-viewport.png"
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
                source: "qrc:///ktouch/images/trainingscreen-viewport-shadow.png"
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
            source: "qrc:///ktouch/images/trainingscreen-footer.png"
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
                    leftMargin: Units.gridUnit
                    rightMargin: Units.gridUnit
                    topMargin: Units.largeSpacing
                    bottomMargin: Units.largeSpacing
                }

                onKeyboardUpdate: {
                    setLessonKeys()
                    highlightedKeys = []
                    updateKeyHighlighting()
                }
            }

            KeyboardUnavailableNotice {
                id: keyboardUnavailableNotice
                colorScheme: colorScheme
                visible: !screen.keyboardLayout.isValid
                width: parent.width
            }
        }
    }

    Item {
        id: trainingOverlayContainer
        anchors.fill: parent
    }

    TrainingScreenMenu {
        id: menu
        onClosed: trainingWidget.forceActiveFocus()
        onRestartRequested: screen.restartRequested()
        onAbortRequested: screen.abortRequested()
    }
}
