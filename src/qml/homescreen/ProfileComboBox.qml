/*
 *  Copyright 20175  Sebastian Gottfried <sebastian.gottfried@posteo.de>
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

import QtQuick 2.6
import QtQuick.Controls 2.2
import QtQuick.Controls.impl 2.2
import QtGraphicalEffects 1.0
import ktouch 1.0

import "../common"

ComboBox {
    id: root

    property Profile profile
    property KColorScheme colorScheme
    property color manageProfileButtonBgColor

    KColorScheme {
        id: listColorScheme
        colorGroup: KColorScheme.Active
        colorSet: KColorScheme.View
    }

    background: Rectangle {
        opacity: (popup.visible? 0.6: 0.0) + (hovered || visualFocus? 0.3: 0.0)
        color: colorScheme.normalBackground
        Behavior on opacity {
            NumberAnimation {
                duration: 150
            }
        }
    }

    hoverEnabled: true

    contentItem: IconLabel {
        icon: "user-identity"
        text: profile !== null? profile.name: ""
        color: colorScheme.normalText

    }

    indicator: MonochromeIcon {
        color: colorScheme.normalText
        icon: "arrow-down-double"
        x: root.width - width - root.contentItem.padding
        y: root.topPadding + (root.availableHeight - height) / 2
        rotation: root.popup.opacity * -180
    }

    model: profileDataAccess.profileCount

    onActivated: {
        profile = index < profileDataAccess.profileCount? profileDataAccess.profile(index): null
    }

    onProfileChanged: {
        for (var i = 0; i < profileDataAccess.profileCount; i++) {
            if (profile === profileDataAccess.profile(i)) {
                if (i != currentIndex) {
                    currentIndex = i;
                }
                break;
            }
        }
    }

    delegate: ListItem {
        width: root.width
        text: index < profileDataAccess.profileCount? profileDataAccess.profile(index).name: ""
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

                ScrollIndicator.vertical: ScrollIndicator { }
                footer: IconButton {
                    id: manageProfileButton
                    width: parent.width
                    color: root.colorScheme.normalText
                    bgColor: root.manageProfileButtonBgColor
                    text: i18n("Manage Profiles")
                    icon: "user-properties"
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: false
                        onPressed: {
                            mouse.accepted = true
                        }
                        onClicked: {
                            console.log("manage profile")
                            root.popup.close()
                            mouse.accepted = true
                        }
                    }
                }
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
