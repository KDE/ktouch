/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *  SPDX-FileCopyrightText: 2015 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtGraphicalEffects 1.0
import ktouch 1.0

Item {
    id: item

    property int keyIndex
    property KeyboardLayout keyboardLayout
    property bool isHighlighted: false
    property bool animateHighlight: true
    property bool enabled: true
    property bool pressed: false
    property real horizontalScaleFactor: 1
    property real verticalScaleFactor: 1

    property AbstractKey key: item.keyboardLayout.key(item.keyIndex)
    property AbstractKey referenceKey: keyboardLayout.referenceKey

    function match(data) {
        var eventText = data
        var eventKey = -1
        if (typeof data === "object") {
            eventText = data.text
            eventKey = data.key
        }
        if (typeof data === "number") {
            eventText = ""
            eventKey = data
        }
        switch (key.keyType()) {
        case "key":
            for (var i = 0; i < key.keyCharCount; i++) {
                if (key.keyChar(i).value == eventText) {
                    return true;
                }
            }
            return false

        case "specialKey":
            switch (key.type) {
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

    function getTint(color) {
        color.a = 0.125
        return color
    }

    property color tint: key && key.keyType() == "key"?
        getTint(preferences.fingerColor(key.fingerIndex)):
        "#00000000"

    x: Math.round(key.left * horizontalScaleFactor)
    y: Math.round(key.top * verticalScaleFactor)
    width: Math.round(key.width * horizontalScaleFactor)
    height: Math.round(key.height * verticalScaleFactor)

    state: enabled? (pressed? "pressed": "normal"): "disabled"

    onIsHighlightedChanged: {
        if (!animateHighlight) {
            shadow.state = isHighlighted? "highlighted1": "normal"
        }
    }

    Rectangle {
        id: shadow

        property int marginSize: 0

        anchors.centerIn: parent
        width: item.width + marginSize
        height: item.height + marginSize
        smooth: true
        radius: body.radius
        state: "normal"

        states: [
            State {
                name: "normal"
                PropertyChanges {
                    target: shadow
                    color: "#000"
                    marginSize: 0
                }
                PropertyChanges {
                    target: shadowEffect
                    glowRadius: body.radius
                }
            },
            State {
                name: "highlighted1"
                PropertyChanges {
                    target: shadow
                    color: "#54A7F0"
                    marginSize: 4
                }
                PropertyChanges {
                    target: shadowEffect
                    glowRadius: 1.5 * body.radius
                }
            },
            State {
                name: "highlighted2"
                PropertyChanges {
                    target: shadow
                    color: "#54A7F0"
                    marginSize: 0
                }
                PropertyChanges {
                    target: shadowEffect
                    glowRadius: 1.5 * body.radius
                }
            }
        ]

        Behavior on marginSize {
            enabled: animateHighlight
            NumberAnimation {
                duration: 150
                easing.type: Easing.InOutQuad
            }
        }

        Behavior on color {
            enabled: animateHighlight
            ColorAnimation { duration: 150 }
        }


        SequentialAnimation {
            id: pulseAnimation
            loops: Animation.Infinite
            running: isHighlighted && animateHighlight
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

    RectangularGlow {
        id: shadowEffect
        anchors.fill: shadow
        color: shadow.color
        glowRadius: 5
        cornerRadius: glowRadius + shadow.radius

        Behavior on glowRadius {
            enabled: animateHighlight
            NumberAnimation {
                duration: 150
                easing.type: Easing.InOutQuad
            }
        }
    }

    Rectangle {
        id: body
        anchors.fill: parent
        radius: Math.max(3, Math.min(referenceKey.height, referenceKey.width) / 10 * Math.min(horizontalScaleFactor, verticalScaleFactor))
        border.width: 1
        border.color: "#000"
        smooth: true

        gradient: Gradient {
            GradientStop { id: gradientStop0; position: 0.0; }
            GradientStop { id: gradientStop1; position: 0.5; }
            GradientStop { id: gradientStop2; position: 1.0; }
        }

        Rectangle {
            id: hapticMarker
            anchors {
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
                bottomMargin: 4
            }
            visible: item.key.keyType() == "key" && item.key.hasHapticMarker
            height: 3
            width: body.width / 3
            radius: 1
            color: topLeftLabel.color
            border {
                width: 1
                color: topLeftLabel.color
            }
        }
    }

    Item {
        anchors.topMargin: Math.max(referenceKey.width / 20, 3) * verticalScaleFactor
        anchors.bottomMargin: anchors.topMargin
        anchors.leftMargin: Math.max(referenceKey.width / 10, 5) * horizontalScaleFactor
        anchors.rightMargin: anchors.leftMargin
        anchors.fill: parent
        KeyLabel {
            id: topLeftLabel
            key: item.key
            position: KeyChar.TopLeft
        }
        KeyLabel {
            id: topRightLabel
            anchors.right: parent.right
            key: item.key
            position: KeyChar.TopRight
        }
        KeyLabel {
            id: bottomLeftLabel
            anchors.bottom: parent.bottom
            key: item.key
            position: KeyChar.BottomLeft
        }
        KeyLabel {
            id: bottomRightLabel
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            key: item.key
            position: KeyChar.BottomRight
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
