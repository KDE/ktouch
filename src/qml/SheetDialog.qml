/*
 *  Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *  Copyright 2015  Sebastian Gottfried <sebastiangottfried@web.de>
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

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import ktouch 1.0

Item {
    id: root

    SystemPalette {
        id: activePalette
        colorGroup: SystemPalette.Active
    }

    property alias content: contentArea.children
    property int innerMargin: 20
    signal opened
    signal closed

    function open() {
        root.state = "open"
        opened()
    }

    function close() {
        root.state = "closed"
        closed()
    }

    function isOpen() {
        return root.state == "open"
    }

    clip: true
    visible: bg.opacity > 0

    /* swallow all mouse events */
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
    }

    Rectangle {
        id: bg
        anchors.fill: parent
        color: activePalette.window
        opacity: 1.0

        Behavior on opacity {
            NumberAnimation {
                duration: 300
            }
        }
    }

    Rectangle {
        id: slider
        color: activePalette.window
        width: parent.width
        height: parent.height

        Item {
            id: contentArea
            anchors {
                fill: parent
            }
        }
    }

    state: "closed"

    states: [
        State {
            name: "open"
            PropertyChanges { target: bg; opacity: 1.0 }
            PropertyChanges { target: slider; y: 0 }
        },
        State {
            name: "closed"
            PropertyChanges { target: bg; opacity: 0 }
            PropertyChanges { target: slider; y: -slider.parent.height }
        }
    ]

    transitions: [
        Transition {
            from: "*"
            to: "*"
            NumberAnimation { target: slider; property: "y"; duration: 300; easing.type: Easing.InOutQuad }
        }
    ]
}
