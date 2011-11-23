import QtQuick 1.0

Item {
    id: line
    property string text
    signal done
    signal keyPressed(variant event)
    signal keyReleased(variant event)

    property string enteredText: ""
    property int position: 0
    property bool isCorrect: true

    focus: true

    onTextChanged: {
        line.enteredText = ""
        line.isCorrect = true
        line.position = 0
        lineChars.model = 0
        lineChars.model = line.text.length
    }

    function deleteLastChar() {
        if (line.position === 0)
            return;
        line.position--;
        var charItem = lineChars.itemAt(line.position);
        charItem.text = line.text.charAt(line.position);
        charItem.state = "placeholder";
        line.enteredText = line.enteredText.substring(0, line.position);
        line.isCorrect = line.enteredText === line.text.substring(0, line.position);
    }

    function addChar(newChar)
    {
        if (line.position >= text.length)
            return;
        var isCorrect = newChar === text.charAt(line.position);
        var charItem = lineChars.itemAt(line.position);
        line.enteredText += newChar;
        line.isCorrect = line.isCorrect && isCorrect;
        if (newChar != " ")
        {
            charItem.text = newChar;
        }
        charItem.state = line.isCorrect? "done": "error";
        line.position++;
    }

    Keys.onPressed: {
        cursorAnimation.restart();
        switch(event.key)
        {
        case Qt.Key_Return:
            if (line.position == text.length && line.isCorrect)
                line.done();
            break;
        case Qt.Key_Backspace:
            deleteLastChar();
            break;
        case Qt.Key_Delete:
        case Qt.Key_Tab:
            break;
        default:
            if (event.text !== "")
            {
                addChar(event.text.charAt(0))
            }
            break;
        }
        if (!event.isAutoRepeat)
            line.keyPressed(event)
    }

    Keys.onReleased: {
        if (!event.isAutoRepeat)
            line.keyReleased(event)
    }

    Row {
        id: row
        anchors.centerIn: parent
        spacing: 0
        Repeater {
            id: lineChars
            model: 0
            Text {
                signal error
                id: lineChar
                font.family: "monospace"
                font.pixelSize: fontSize
                transformOrigin: Item.Center
                text: line.text.charAt(index)
                state: "placeholder"
                states: [
                    State {
                        name: "placeholder"
                        PropertyChanges {
                            target: lineChar
                            color: "#888"
                        }
                    },
                    State {
                        name: "done"
                        PropertyChanges {
                            target: lineChar
                            color: "#000"
                        }
                    },
                    State {
                        name: "error"
                        PropertyChanges {
                            target: lineChar
                            color: "#f00"
                        }
                    }
                ]
                onStateChanged: {
                    if (lineChar.state == "error")
                        error();
                }

                onError: SequentialAnimation {
                    NumberAnimation {
                        target: lineChar
                        property: "scale"
                        to: 1.3
                        duration: 100
                        easing.type: Easing.OutBack
                        easing.overshoot: 20
                    }
                    NumberAnimation {
                        target: lineChar
                        property: "scale"
                        to: 1.0
                        duration: 50
                        easing.type: Easing.InCubic
                    }
                }
            }
        }
    }
    Rectangle {
        id: cursor
        property Item target: lineChars.itemAt(line.position - 1)
        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter
            leftMargin: (target? target.x + target.width: 0) + row.x
        }
        width: 1
        height: fontSize * 1.2
        color: "#000"
        visible: line.activeFocus
        SequentialAnimation {
            id: cursorAnimation
            running: line.activeFocus
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
