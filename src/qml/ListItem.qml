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
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

Item {
    id: root
    property alias title: label.text
    property string iconSource
    property alias label: label
    signal clicked
    signal doubleClicked
    height: padding.height + bg.margins.top + bg.margins.bottom
    state: ListView.isCurrentItem? "selected": mouseArea.containsMouse? "hover": "normal"

    PlasmaCore.FrameSvgItem {
        id: bg
        imagePath: "widgets/viewitem"
        prefix: "hover"
        opacity: 0
        anchors.fill: parent
    }

    Item {
        id: padding

        anchors {
            fill: parent
            topMargin: bg.margins.top
            rightMargin: bg.margins.right
            bottomMargin: bg.margins.bottom
            leftMargin: bg.margins.left
        }

        width: parent.width
        height: Math.max(label.height, label.height)

        QIconItem {
            id: iconItem
            visible: !!root.iconSource
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            icon: QIcon(root.iconSource)
            width: theme.smallIconSize
            height: theme.smallIconSize
        }

        PlasmaComponents.Label {
            id: label
            elide: Text.ElideRight
            anchors {
                left: iconItem.visible? iconItem.right: parent.left
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
            height: paintedHeight
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.clicked()
        onDoubleClicked: root.doubleClicked()
    }

    states: [
        State {
            name: "normal"
            PropertyChanges {
                target: bg
                opacity: 0
            }
        },
        State {
            name: "hover"
            PropertyChanges {
                target: bg
                opacity: 1
                prefix: "hover"
            }
        },
        State {
            name: "selected"
            PropertyChanges {
                target: bg
                opacity: 1
                prefix: "selected"
            }
        }
    ]

    transitions: [
        Transition {
            from: "normal"
            to: "hover"
            NumberAnimation {
                target: bg
                property: "opacity"
                duration: 250
                easing.type: Easing.OutCubic
            }
        },
        Transition {
            from: "hover"
            to: "normal"
            NumberAnimation {
                target: bg
                property: "opacity"
                duration: 250
                easing.type: Easing.OutCubic
            }
        }
    ]
}
