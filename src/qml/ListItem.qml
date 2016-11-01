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
import org.kde.kquickcontrolsaddons 2.0
import ktouch 1.0

Item {
    id: root
    property alias title: label.text
    property string iconSource
    property alias label: label
    property bool isCurrent: ListView.isCurrentItem;
    signal clicked
    signal doubleClicked
    height: 2 * content.height

    SystemPalette {
        id: listItemPallete
        colorGroup: SystemPalette.Active
    }

    Rectangle {
        id: bg
        anchors.fill: parent
        color: Qt.rgba(listItemPallete.highlight.r, listItemPallete.highlight.g, listItemPallete.highlight.b, 0.3)
        radius: 0.2 * height
        border {
            width: 1
            color: listItemPallete.highlight
        }
        opacity: root.isCurrent || mouseArea.containsMouse? 1: 0
        Behavior on opacity {
            NumberAnimation {
                duration: 150
            }
        }
    }

    Item {
        id: content

        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: parent.right
            leftMargin: 5
            rightMargin: 5
        }

        height: Math.max(label.height, label.height)

        QIconItem {
            id: iconItem
            visible: !!root.iconSource
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            icon: root.iconSource
            width: 22
            height: 22
        }

        Label {
            id: label
            elide: Text.ElideRight
            anchors {
                left: iconItem.visible? iconItem.right: parent.left
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.clicked()
        onDoubleClicked: root.doubleClicked()
    }
}
