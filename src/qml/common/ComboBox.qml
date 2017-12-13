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

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import ktouch 1.0

ComboBox {
    id: control

    property KColorScheme colorScheme: KColorScheme {
        colorGroup: control.enabled? KColorScheme.Active: KColorScheme.Inactive
        colorSet: KColorScheme.Button
    }

    property alias popupListView: list

    KColorScheme {
        id: listColorScheme
        colorGroup: KColorScheme.Active
        colorSet: KColorScheme.View
    }

    hoverEnabled: true

    background: Item {
        Rectangle {
            anchors.fill: parent
            opacity: (popup.visible? 0.6: 0.0) + (root.hovered || root.visualFocus? 0.3: 0.0)
            color: colorScheme.normalBackground
            Behavior on opacity {
                NumberAnimation {
                    duration: 150
                }
            }
        }

        FocusBar {
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            height: 3
            control: root
        }
    }

    contentItem: Label {
        text: control.displayText
        color: colorScheme.normalText
    }

    indicator: MonochromeIcon {
        color: colorScheme.normalText
        icon: "arrow-down-double"
        x: root.width - width - root.contentItem.padding
        y: root.topPadding + (root.availableHeight - height) / 2
        rotation: root.popup.opacity * -180
    }

    delegate: ListItem {
        width: root.width
        text: modelData
        highlighted: root.currentIndex === index
    }

    popup: Popup {
        y: root.height
        width: root.width
        implicitHeight: contentItem.implicitHeight
        padding: 0
        opacity: 0

        enter: Transition {
            NumberAnimation {
                property: "opacity"
                to: 1.0
                duration: 150
            }
        }

        exit: Transition {
            NumberAnimation {
                property: "opacity"
                to: 0.0
                duration: 150
            }
        }

        contentItem:
            ListView {
                id: list
                clip: true
                implicitHeight: contentHeight
                width: parent.width
                model: root.popup.visible ? root.delegateModel : null
                currentIndex: root.highlightedIndex
            }

        background: Rectangle {
            color: listColorScheme.normalBackground
            layer.enabled: true
            layer.effect: DropShadow {
                samples: 24
                horizontalOffset: 0
                verticalOffset: 0
            }
        }
    }
}
