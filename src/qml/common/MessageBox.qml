/*
 *  Copyright 2018  Sebastian Gottfried <sebastian.gottfried@posteo.de>
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

import QtQuick 2.9

import "../common"

Rectangle {
    id: item
    anchors.verticalCenter: parent.verticalCenter
    radius: 3
    color: "#eee4be"
    height: label.height + 6
    width: label.width + 6
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
        id: label
        padding: 2
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
