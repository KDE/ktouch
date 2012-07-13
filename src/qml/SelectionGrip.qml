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
import ktouch 1.0

Rectangle {
    property string horizontalPosition
    property string verticalPosition
    width: 5
    height: 5
    color: "navy"
    anchors {
        left: horizontalPosition == "left"? parent.left: null
        horizontalCenter: horizontalPosition == "center"? parent.horizontalCenter: null
        right: horizontalPosition == "right"? parent.right: null
        top: verticalPosition == "top"? parent.top: null
        verticalCenter: verticalPosition == "center"? parent.verticalCenter: null
        bottom: verticalPosition == "bottom"? parent.bottom: null
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            mouse.accepted = true
        }
    }
}