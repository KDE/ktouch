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
import Effects 1.0
import ktouch 1.0

Item
{
    id: item

    property int keyIndex
    property KeyboardLayout keyboardLayout
    property color tint: "#00000000"
    property bool isHighlighted: false
    property bool enabled: true
    property bool pressed: false

    property AbstractKey key: item.keyboardLayout.key(item.keyIndex)
    property AbstractKey referenceKey: keyboardLayout.referenceKey

    function match(data)
    {
        var eventText = data
        var eventKey = -1
        if (typeof data === "object")
        {
            eventText = data.text
            eventKey = data.key
        }
        if (typeof data === "number")
        {
            eventText = ""
            eventKey = data
        }
        switch (key.keyType())
        {
        case "key":
            for (var i = 0; i < key.keyCharCount; i++)
            {
                if (key.keyChar(i).value == eventText.charCodeAt(0))
                {
                    return true;
                }
            }
            return false

        case "specialKey":
            switch (key.type)
            {
            case SpecialKey.Tab:
                return eventKey == Qt.Key_Tab
            case SpecialKey.Capslock:
                return eventKey == Qt.Key_CapsLock
            case SpecialKey.Shift:
                return eventKey == Qt.Key_Shift
            case SpecialKey.Backspace:
                return eventKey == Qt.Key_Backspace
            case SpecialKey.Return:
                return eventKey == Qt.Key_Return
            case SpecialKey.Space:
                return eventKey == Qt.Key_Space || eventText == " "
            }
            return false
        }
        return false;
    }

    function setTint(color)
    {
        // stupid hack to set alpha because in Qt Quick 1.1 it's impossible to access color components
        color = "#20" + ("" + color).substr(1);
        item.tint = color

    }

    anchors.left: parent.left
    anchors.top: parent.top
    anchors.leftMargin: Math.round(key.left * scaleFactor)
    anchors.topMargin: Math.round(key.top * scaleFactor)
    width: Math.round(key.width * scaleFactor)
    height: Math.round(key.height * scaleFactor)

    state: enabled? (pressed? "pressed": "normal"): "disabled"

    onKeyChanged: {
        switch (key.keyType())
        {
        case "key":
            for (var i = 0; i < item.key.keyCharCount; i++)
            {
                var keyChar = item.key.keyChar(i);
                var position = keyChar.position;
                if (position === KeyChar.TopLeft)
                {
                    topLeftLabel.keyChar = keyChar;
                }
                else if (position === KeyChar.TopRight)
                {
                    topRightLabel.keyChar = keyChar;
                }
                else if (position === KeyChar.BottomLeft)
                {
                    bottomLeftLabel.keyChar = keyChar;
                }
                else if (position === KeyChar.BottomRight)
                {
                    bottomRightLabel.keyChar = keyChar;
                }
            }
            setTint(preferences.fingerColor(key.fingerIndex))
            break;

        case "specialKey":
            switch (key.type)
            {
            case SpecialKey.Other:
                topLeftLabel.text = key.label
                break;
            case SpecialKey.Tab:
                topLeftLabel.text = "\u21B9"
                break;
            case SpecialKey.Capslock:
                topLeftLabel.text = "\u21E9"
                break;
            case SpecialKey.Shift:
                topLeftLabel.text = "\u21E7"
                break;
            case SpecialKey.Backspace:
                topLeftLabel.text = "\u2190"
                break;
            case SpecialKey.Return:
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

            Behavior on blurRadius {
                NumberAnimation {
                    duration: 150
                    easing.type: Easing.InOutQuad
                }
            }
        }

        state: "normal"

        states: [
            State {
                name: "normal"
                PropertyChanges {
                    target: shadow
                    color: "#000"
                    width: item.width
                    height: item.height
                }
                PropertyChanges {
                    target: shadow.effect
                    blurRadius: 10
                }
            },
            State {
                name: "highlighted1"
                PropertyChanges {
                    target: shadow
                    color: "#54A7F0"
                    width: item.width + 4
                    height: item.height + 4
                }
                PropertyChanges {
                    target: shadow.effect
                    blurRadius: 15
                }
            },
            State {
                name: "highlighted2"
                PropertyChanges {
                    target: shadow
                    color: "#54A7F0"
                    width: item.width
                    height: item.height
                }
                PropertyChanges {
                    target: shadow.effect
                    blurRadius: 15
                }
            }
        ]

        Behavior on width {
            NumberAnimation {
                duration: 150
                easing.type: Easing.InOutQuad
            }
        }

        Behavior on height {
            NumberAnimation {
                duration: 150
                easing.type: Easing.InOutQuad
            }
        }

        Behavior on color {
            ColorAnimation { duration: 150 }
        }


        SequentialAnimation {
            id: pulseAnimation
            loops: Animation.Infinite
            running: isHighlighted
            onRunningChanged: {
                if (!running)
                    shadow.state = "normal"
            }

            ScriptAction {
                script: shadow.state = "highlighted1"
            }
            PauseAnimation { duration: 850 }
            ScriptAction {
                script: shadow.state = "highlighted2"
            }
            PauseAnimation { duration: 150 }
        }

    }

    Rectangle
    {
        id: body
        anchors.fill: parent
        radius: Math.max(3, Math.min(referenceKey.height, referenceKey.width) / 10 * scaleFactor)
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
        anchors.topMargin: Math.max(referenceKey.width / 20, 3 * scaleFactor)
        anchors.bottomMargin: anchors.topMargin
        anchors.leftMargin: Math.max(referenceKey.width / 10, 5 * scaleFactor)
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

    states: [
        State {
            name: "normal"
            PropertyChanges {
                target: gradientStop0
                color: Qt.tint("#f0f0f0", item.tint)
            }
            PropertyChanges {
                target: gradientStop1
                color: Qt.tint("#d5d5d5", item.tint)
            }
            PropertyChanges {
                target: gradientStop2
                color: Qt.tint("#ccc", item.tint)
            }
        },
        State {
            name: "pressed"
            PropertyChanges {
                target: gradientStop0
                color: Qt.tint("#666", item.tint)
            }
            PropertyChanges {
                target: gradientStop1
                color: Qt.tint("#888", item.tint)
            }
            PropertyChanges {
                target: gradientStop2
                color: Qt.tint("#999", item.tint)
            }
        },
        State {
            name: "disabled"
            PropertyChanges {
                target: gradientStop0
                color: Qt.tint("#444", item.tint)
            }
            PropertyChanges {
                target: gradientStop1
                color: Qt.tint("#333", item.tint)
            }
            PropertyChanges {
                target: gradientStop2
                color: Qt.tint("#222", item.tint)
            }
        }
    ]
}
