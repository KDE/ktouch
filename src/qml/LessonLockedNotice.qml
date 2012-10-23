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
import org.kde.qtextracomponents 0.1
import org.kde.plasma.components 0.1 as PlasmaComponents

Item {
    id: root

    width: content.width + 40
    height: content.height + 40

    Rectangle {
        anchors.fill: parent
        id: background
        color: "#000"
        opacity: 0.7
        radius: 15
    }

    Behavior on opacity {
        NumberAnimation {
            duration: 200
        }
    }

    Column {
        id: content

        anchors.centerIn: parent
        width: Math.max(icon.width, text.width)
        spacing: 10

        QIconItem {
            id: icon
            anchors.horizontalCenter: parent.horizontalCenter
            icon: QIcon("object-locked")
            width: theme.enormousIconSize
            height: theme.enormousIconSize
        }

        PlasmaComponents.Label {
            id: text
            text: i18n("Complete Previous Lessons to Unlock")
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            color: "#fff"
            font.weight: Font.Bold
            wrapMode: Text.Wrap
        }
    }
}
