/*
 *  SPDX-FileCopyrightText: 2018 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9

import "../common"

Rectangle {
    id: item
    anchors.verticalCenter: parent.verticalCenter
    radius: Units.smallSpacing
    color: "#eee4be"
    height: label.height
    width: label.width
    smooth: true

    function showMessage(msg, iconSource) {
        item.state = "hidden";
        label.text = msg
        label.iconName = iconSource || ""
        item.state = "normal"
    }

    function clearMessage() {
        item.state = "cleared"
    }

    function clearMessageImmediately() {
        item.state = "hidden"
    }

    border {
        width: 1
        color: "#decd87"
    }

    transform: Scale {
        id: itemScaler
        origin.x: 0
        origin.y: 0
    }

    state: "hidden"

    IconLabel {
        anchors.centerIn: parent
        padding: Units.smallSpacing
        id: label
    }

    states: [
        State {
            name: "normal"
            PropertyChanges {
                target: itemScaler
                xScale: 1
            }
        },
        State {
            name: "hidden"
            PropertyChanges {
                target: itemScaler
                xScale: 0
            }
        },
        State {
            name: "cleared"
            PropertyChanges {
                target: itemScaler
                xScale: 0
            }
        }
    ]

    transitions: [
        Transition {
            from: "*"
            to: "normal"
            NumberAnimation {
                target: itemScaler
                property: "xScale"
                duration: 500
                easing.type: Easing.InOutQuad
            }
        },
        Transition {
            from: "normal"
            to: "cleared"
            NumberAnimation {
                target: itemScaler
                property: "xScale"
                duration: 500
                easing.type: Easing.InOutQuad
            }
        }
    ]
}
