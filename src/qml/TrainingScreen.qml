import QtQuick 1.0

Rectangle {
    property Key highlightedKey
    id: screen
    color: "#ccc"

    Column {
        anchors.fill: parent
        Rectangle {
            id: head
            color: "#4f4f4f"
            width: parent.width
            height: 60
            Text {
                anchors.centerIn: parent
                color: "#666"
                text: "header"
            }
        }

        TrainingWidget {
            id: body
            width: parent.width
            height: Math.round((parent.height - head.height) / 2)
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
        Keyboard {
            id: keyboard
            width: parent.width
            height: parent.height - head.height - body.height
        }
    }
}
