import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import ktouch 1.0

Item {

    id: screen

    property KeyboardLayout keyboardLayout
    property Lesson lesson

    signal restartRequested()
    signal abortRequested()

    property bool trainingStarted: false
    property bool trainingFinished: true
    property KeyItem highlightedKey
    property bool isActive

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
                    if (chars.indexOf(String.fromCharCode(keyChar.value)) != -1) {
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
    }

    function start() {
        screen.trainingFinished = false
        screen.trainingStarted = true
    }

    onLessonChanged: setLessonKeys()

    TrainingStats {
        id: stats
        onTimeIsRunningChanged: {
            if (timeIsRunning)
                screen.trainingStarted = false
        }
    }

    PlasmaCore.Svg {
        id: screenSvg
        imagePath: findImage("trainingscreen.svgz")
        usingRenderingCache: false
    }

    Column {
        anchors.fill: parent

        PlasmaCore.SvgItem {
            id: header
            svg: screenSvg
            elementId: "header"
            width: parent.width
            height: 120
            Row {
                anchors.centerIn: parent
                spacing: 10
                EllapsedTimeMeter {
                    ellapsedTime: stats.ellapsedTime
                }
                CharactersPerMinuteMeter {
                    charactersPerMinute: stats.charactersPerMinute
                }
                AccuracyMeter {
                    accuracy: stats.accuracy
                }
            }
        }

        TrainingScreenToolbar {
            id: toolbar
            height: 29
            width: parent.width
            trainingStarted: screen.trainingStarted
            trainingFinished: screen.trainingFinished
            menuOverlayItem: menuOverlay
        }

        PlasmaCore.SvgItem {
            id: body
            width: parent.width
            height: parent.height - toolbar.height - header.height - footer.height
            svg: screenSvg
            elementId: "content"

            TrainingWidget {
                id: trainingWidget
                anchors.fill: parent
                lesson: screen.lesson
                onKeyPressed: keyboard.handleKeyPress(event)
                onKeyReleased: keyboard.handleKeyRelease(event)
                onNextCharChanged: {
                    if (!isCorrect)
                        return;
                    if (highlightedKey)
                        highlightedKey.isHighlighted = false
                    var key = nextChar != ""? keyboard.findKeyItem(nextChar): keyboard.findKeyItem(Qt.Key_Return)
                    if (key)
                    {
                        key.isHighlighted = true
                        highlightedKey = key
                    }
                }
                onIsCorrectChanged: {
                    if (!isCorrect)
                    {
                        if (highlightedKey)
                            highlightedKey.isHighlighted = false
                        var key = keyboard.findKeyItem(Qt.Key_Backspace)
                        if (key)
                        {
                            key.isHighlighted = true
                            highlightedKey = key
                        }
                    }
                }
                onFinished: screen.trainingFinished = true
            }

            PlasmaCore.FrameSvgItem {
                anchors.fill: parent
                imagePath: findImage("trainingscreen.svgz")
                prefix: "content-shadow"
            }
        }

        PlasmaCore.SvgItem {
            id: footer
            width: parent.width
            visible: preferences.showKeyboard
            height: visible? Math.round(Math.min((parent.height - toolbar.height - header.height) / 2, parent.width / keyboard.aspectRatio)): 0
            svg: screenSvg
            elementId: "footer"
            Keyboard {
                keyboardLayout: screen.keyboardLayout
                id: keyboard
                anchors.fill: parent
                onKeyboardUpdate: setLessonKeys()
            }
        }
    }

    TrainingScreenMenuOverlay {
        id: menuOverlay
        anchors.fill: parent
        onVisibleChanged: {
            if (!visible) {
                trainingWidget.forceActiveFocus()
            }
        }
        onRestartRequested: screen.restartRequested()
        onAbortRequested: screen.abortRequested()
    }

    Binding {
        target: screen
        property: "isActive"
        value: Qt.application.active
    }

    onIsActiveChanged: {
        if (!screen.isActive) {
            stats.stopTraining()
        }
    }
}
