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
import org.kde.kquickcontrolsaddons 2.0

Rectangle {
    id: item
    anchors.verticalCenter: parent.verticalCenter
    radius: 3
    color: "#eee4be"
    height: layout.height + 6
    width: layout.width + 6
    smooth: true

    function showMessage(msg, iconSource) {
        item.state = "hidden";
        label.text = msg
        icon.icon = iconSource || ""
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

    Row {
        id: layout
        anchors.centerIn: parent
        width: icon.width + spacing + label.width
        spacing: icon.valid? 3: 0

        QIconItem {
            id: icon
            width: height
            height: icon? label.height: 0
        }

        Label {
            id: label
            color: "#000000"
        }
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
