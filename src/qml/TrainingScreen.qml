import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import ktouch 1.0

Item {

    id: screen

    property KeyboardLayout keyboardLayout
    property Lesson lesson

    property KeyItem highlightedKey
    property bool isActive

    TrainingStats {
        id: stats
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

        PlasmaCore.SvgItem {
            id: body
            width: parent.width
            height: parent.height - header.height - footer.height
            svg: screenSvg
            elementId: "content"

            TrainingWidget {
                anchors.fill: parent
                lesson: screen.lesson
                onKeyPressed: keyboard.handleKeyPress(event)
                onKeyReleased: keyboard.handleKeyRelease(event)
                onNextCharChanged: {
                    if (!isCorrect)
                        return;
                    if (highlightedKey)
                        highlightedKey.isHighlighted = false
                    var key = nextChar != ""? keyboard.findKey(nextChar): keyboard.findKey(Qt.Key_Return)
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
                        var key = keyboard.findKey(Qt.Key_Backspace)
                        if (key)
                        {
                            key.isHighlighted = true
                            highlightedKey = key
                        }
                    }
                }
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
            height: visible? Math.round(Math.min((parent.height - header.height) / 2, parent.width / keyboard.aspectRatio)): 0
            svg: screenSvg
            elementId: "footer"
            Keyboard {
                keyboardLayout: screen.keyboardLayout
                id: keyboard
                anchors.fill: parent
                visible: parent.visible
            }
        }
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
