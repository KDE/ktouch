/*
 *  Copyright 2017  Sebastian Gottfried <sebastian.gottfried@posteo.de>
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
import QtQuick.Controls 2.2

Label {
    property string icon: ""
    property bool reserveSpaceForIcon: false
    id: label
    elide: Text.ElideRight

    padding: Math.ceil(0.7 * font.pixelSize)
    leftPadding: (iconItem.visible || reserveSpaceForIcon? padding + iconItem.width: 0) + (label.text !== ""? padding: 0)
    verticalAlignment: Text.AlignVCenter

    MonochromeIcon {
        id: iconItem
        visible: label.icon != ""
        color: label.color
        anchors {
            left: parent.left
            leftMargin: label.text === ""? (label.width - width) / 2: label.padding
            verticalCenter: parent.verticalCenter
        }
        icon: label.icon
        width: 22
        height: 22
    }
}
