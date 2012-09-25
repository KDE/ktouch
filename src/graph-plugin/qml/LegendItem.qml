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
import org.kde.plasma.components 0.1 as PlasmaComponents


Item {
    property Dimension dimension

    height: label.height
    width: box.width + label.width + 5

    Rectangle {
        id: box
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
        }
        color: dimension.color
        width: height
        height: label.font.pixelSize
        radius: 4
    }

    PlasmaComponents.Label {
        id: label
        anchors {
            top: parent.top
            right: parent.right
        }
        text: dimension.label
        height: paintedHeight
    }
}
