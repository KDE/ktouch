import QtQuick 1.0
import Effects 1.0
import ktouch 1.0

Item
{
    id: key

    property color tint: "#00000000"

    property AbstractKeyModel keyModel
    property AbstractKeyModel referenceKeyModel

    function match(event)
    {
        var key = event.key
        switch (keyModel.keyType())
        {
        case "key":
            for (var i = 0; i < keyModel.keyCharCount(); i++)
            {
                if (keyModel.keyChar(i).value == event.text)
                {
                    return true;
                }
            }
            return false

        case "specialKey":
            switch (keyModel.type)
            {
            case SpecialKeyModel.Tab:
                return key == Qt.Key_Tab
            case SpecialKeyModel.Capslock:
                return key == Qt.Key_CapsLock
            case SpecialKeyModel.Shift:
                return key == Qt.Key_Shift
            case SpecialKeyModel.Backspace:
                return key == Qt.Key_Backspace
            case SpecialKeyModel.Return:
                return key == Qt.Key_Return
            case SpecialKeyModel.Space:
                return key == Qt.Key_Space
            }
            return false
        }
        return false;
    }

    anchors.left: parent.left
    anchors.top: parent.top
    anchors.leftMargin: keyModel.left * scaleFactor
    anchors.topMargin: keyModel.top * scaleFactor
    width: keyModel.width * scaleFactor
    height: keyModel.height * scaleFactor

    state: "normal"

    onKeyModelChanged: {
        switch (keyModel.keyType())
        {
        case "key":
            for (var i = 0; i < key.keyModel.keyCharCount(); i++)
            {
                var keyCharModel = key.keyModel.keyChar(i);
                var position = keyCharModel.position;
                if (position === KeyCharModel.TopLeft)
                {
                    topLeftLabel.keyCharModel = keyCharModel;
                }
                else if (position === KeyCharModel.TopRight)
                {
                    topRightLabel.keyCharModel = keyCharModel;
                }
                else if (position === KeyCharModel.BottomLeft)
                {
                    bottomLeftLabel.keyCharModel = keyCharModel;
                }
                else if (position === KeyCharModel.BottomRight)
                {
                    bottomRightLabel.keyCharModel = keyCharModel;
                }
            }
            break;

        case "specialKey":
            switch (keyModel.type)
            {
            case SpecialKeyModel.Other:
                topLeftLabel.text = keyModel.label
                break;
            case SpecialKeyModel.Tab:
                topLeftLabel.text = "\u21B9"
                break;
            case SpecialKeyModel.Capslock:
                topLeftLabel.text = "\u21E9"
                break;
            case SpecialKeyModel.Shift:
                topLeftLabel.text = "\u21E7"
                break;
            case SpecialKeyModel.Backspace:
                topLeftLabel.text = "\u2190"
                break;
            case SpecialKeyModel.Return:
                topLeftLabel.text = "\u21B5"
                break;
            }
            break;
        }


    }

    Rectangle
    {
        id: shadow

        anchors.centerIn: parent
        smooth: true
        radius: body.radius
        effect: DropShadow {
            color: shadow.color
            blurRadius: 5
            xOffset: 0
            yOffset: 0
        }
        state: "normal"
        states: [
            State {
                name: "normal"
                PropertyChanges {
                    target: shadow
                    color: "#000"
                    width: parent.width
                    height: parent.height
                }
                PropertyChanges {
                    target: shadow.effect
                    blurRadius: 10
                }
            },
            State {
                name: "highlighted"
                PropertyChanges {
                    target: shadow
                    color: "#54A7F0"
                    width: parent.width + 4
                    height: parent.height + 4
                }
                PropertyChanges {
                    target: shadow.effect
                    blurRadius: 15
                }
            }
        ]
        
        
        transitions: [
            Transition {
                from: "normal"
                to: "highlighted"
                ParallelAnimation {
                    ColorAnimation {
                        target: shadow
                        duration: 50
                        easing.type: Easing.OutCubic
                    }
                    NumberAnimation {
                        target: shadow
                        properties: "width, height"
                        duration: 150
                        easing.type: Easing.OutBack
                        easing.overshoot: 5
                    }
                    NumberAnimation {
                        target: shadow.effect
                        properties: "blurRadius"
                        duration: 50
                        easing.type: Easing.OutCubic
                    }
                }
            },
            Transition {
                from: "highlighted"
                to: "normal"
                ParallelAnimation {
                    ColorAnimation {
                        target: shadow
                        duration: 150
                        easing.type: Easing.InCubic
                    }
                    NumberAnimation {
                        target: shadow
                        properties: "width, height"
                        duration: 150
                        easing.type: Easing.InCubic
                    }
                    NumberAnimation {
                        target: shadow.effect
                        properties: "blurRadius"
                        duration: 150
                        easing.type: Easing.InCubic
                    }
                }
            }
        ]

    }

    Rectangle
    {
        id: body
        anchors.fill: parent
        radius: Math.max(3, Math.min(referenceKeyModel.height, referenceKeyModel.width) / 10 * scaleFactor)
        border.width: 1
        border.color: "#000"
        smooth: true

        gradient: Gradient
        {
            GradientStop { id: gradientStop0; position: 0.0; }
            GradientStop { id: gradientStop1; position: 0.5; }
            GradientStop { id: gradientStop2; position: 1.0; }
        }
    }

    Item
    {
        anchors.topMargin: Math.max(referenceKeyModel.width / 20, 3 * scaleFactor)
        anchors.bottomMargin: anchors.topMargin
        anchors.leftMargin: Math.max(referenceKeyModel.width / 10, 5 * scaleFactor)
        anchors.rightMargin: anchors.leftMargin
        anchors.fill: parent
        KeyLabel
        {
            id: topLeftLabel
        }
        KeyLabel
        {
            id: topRightLabel
            anchors.right: parent.right
        }
        KeyLabel
        {
            id: bottomLeftLabel
            anchors.bottom: parent.bottom
        }
        KeyLabel
        {
            id: bottomRightLabel
            anchors.right: parent.right
            anchors.bottom: parent.bottom
        }
    }

    MouseArea {
        anchors.fill: body
        onPressed: {
            key.state = "pressed"
        }
        onReleased: {
            key.state = "normal"
        }
        onClicked: {
            shadow.state = shadow.state == "normal"? "highlighted": "normal"
        }
    }

    states: [
        State {
            name: "normal"
            PropertyChanges {
                target: gradientStop0
                color: Qt.tint("#f0f0f0", key.tint)
            }
            PropertyChanges {
                target: gradientStop1
                color: Qt.tint("#d5d5d5", key.tint)
            }
            PropertyChanges {
                target: gradientStop2
                color: Qt.tint("#ccc", key.tint)
            }
        },
        State {
            name: "pressed"
            PropertyChanges {
                target: gradientStop0
                color: Qt.tint("#666", key.tint)
            }
            PropertyChanges {
                target: gradientStop1
                color: Qt.tint("#888", key.tint)
            }
            PropertyChanges {
                target: gradientStop2
                color: Qt.tint("#999", key.tint)
            }
        }
    ]
}
