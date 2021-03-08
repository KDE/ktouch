/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtGraphicalEffects 1.0
import ktouch 1.0

import "../common"

ComboBox {
    id: root

    property Profile profile
    property color manageProfileButtonBgColor

    model: profileDataAccess.profileCount

    contentItem: IconLabel {
        iconName: "user-identity"
        text: profile !== null? profile.name: ""
        color: colorScheme.normalText

    }

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

    popupListView.footer:  IconButton {
        id: manageProfileButton
        width: parent.width
        color: root.colorScheme.normalText
        bgColor: root.manageProfileButtonBgColor
        text: i18n("Manage Profiles")
        iconName: "user-properties"
        MouseArea {
            anchors.fill: parent
            hoverEnabled: false
            onPressed: {
                mouse.accepted = true
            }
            onClicked: {
                root.popup.close()
                mouse.accepted = true
                manageProfileDialog.open()
            }
        }
    }

    PopupDialog {
        id: manageProfileDialog

        margins: {
            left: 2 * Units.gridUnit
            bottom: 2 * Units.gridUnit
            right: 2 * Units.gridUnit
            top: 2 * Units.gridUnit
        }

        width: parent.width - leftMargin - rightMargin
        height: parent.height - topMargin - bottomMargin
        modal: true
        focus: true
        title: i18nc("@title:window", "Manage Profiles")
        closePolicy: PopupDialog.CloseOnEscape
        padding: 0
        contentItem:  ProfileSelector {
            id: profileSelector
            onProfileChosen: {
                root.profile = profile
                manageProfileDialog.close()
            }
        }
        onOpened: {
            if (profileComboBox.profile) {
                var index = profileDataAccess.indexOfProfile(profileComboBox.profile)
                profileSelector.selectProfile(index)
            }
        }
    }
}
