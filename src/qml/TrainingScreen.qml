import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore

Rectangle {
    property Key highlightedKey
    id: screen
    color: "#ccc"

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
            height: 60
            Text {
                anchors.centerIn: parent
                color: "#fff"
                text:
                    "time: " + Qt.formatTime(stats.ellapsedTime, "mm:ss") + "\n" +
                    "errors: " + stats.errorCount + "\n" +
                    "accuracy: " + (Math.round(1000 * stats.accuracy) / 10) + "\n" +
                    "charactersPerMinute: " + stats.charactersPerMinute
            }
        }

        PlasmaCore.SvgItem {
            id: body
            width: parent.width
            height: parent.height - header.height - footer.height
            svg: screenSvg
            elementId: "body"

            TrainingWidget {
                anchors.fill: parent
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

            PlasmaCore.SvgItem {
                anchors.fill: parent
                svg: screenSvg
                elementId: "body-shadow"
            }
        }
        PlasmaCore.SvgItem {
            id: footer
            width: parent.width
            height: Math.round(Math.min((parent.height - header.height) / 2, parent.width / keyboard.aspectRatio))
            svg: screenSvg
            elementId: "footer"
            Keyboard {
                id: keyboard
                anchors.fill: parent
            }
        }
    }
}
