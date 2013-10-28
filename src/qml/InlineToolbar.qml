/*
 *  Copyright 2013  Sebastian Gottfried <sebastiangottfried@web.de>
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
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

Item {

    id: root

    property alias content: contentArea.data
    property alias color: background.color

    width: contentArea.width + 6
    height: contentArea.height + 6

    Rectangle {
        anchors.fill: parent
        id: background
        color: "#888"
        opacity: 0.7
        radius: 3
    }

    Behavior on opacity {
        NumberAnimation { duration: 250 }
    }

    Row {
        id: contentArea
        anchors.centerIn: parent
        height: childrenRect.height
        spacing: 3
    }
}
