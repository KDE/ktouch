/*
 *  Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *  Copyright 2015  Sebastian Gottfried <sebastiangottfried@web.de>
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
import QtQuick.Layouts 1.3
import ktouch 1.0

import "../common"

FocusScope {
    id: root

    signal profileChosen(variant profile)

    function createNewProfile() {
        var profile = profileDataAccess.createProfile()
        profileForm.profile = profile
    }

    function selectProfile(index) {
        list.currentIndex = index
        profileForm.profile = profileDataAccess.profile(index)
    }

    KColorScheme {
        id: listColorScheme
        colorGroup: KColorScheme.Active
        colorSet: KColorScheme.View
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 20

            ListView {
                id: list
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: profileDataAccess.profileCount + 1
                clip: true
                focus: true
                delegate: ListItem {
                    property bool isNewButton: index >= profileDataAccess.profileCount
                    width: list.width
                    text: isNewButton?
                            i18n("Create New Profile"):
                            index < profileDataAccess.profileCount? profileDataAccess.profile(index).name: null
                    label.font.italic: isNewButton
                    iconName: isNewButton? "list-add": "user-identity"
                    highlighted: ListView.isCurrentItem
                    onClicked: {
                        list.forceActiveFocus()
                        list.currentIndex = index
                        if (isNewButton) {
                            createNewProfile()
                        }
                        else {
                            selectProfile(index)
                        }
                    }
                    onDoubleClicked: {
                        if (!isNewButton) {
                            root.profileChosen(profileDataAccess.profile(list.currentIndex))
                        }
                    }
                }
                onCurrentItemChanged: {
                    if (currentItem.isNewButton) {
                        createNewProfile()
                    }
                    else {
                        selectProfile(currentIndex)
                    }
                }
            }

            ProfileDetailsItem {
                id: profileForm
                Layout.fillWidth: true
                Layout.fillHeight: true

                onDeletionRequest: {
                    var index = profileDataAccess.indexOfProfile(profileForm.profile)
                    profileForm.profile = null
                    profileDataAccess.removeProfile(index)
                    selectProfile(Math.max(0, list.currentIndex - 1))
                }
            }
        }

        IconButton {
            id: selectButton
            Layout.fillWidth: true
            iconName: "go-next-view"
            text: i18n("Use Selected Profile")
            enabled: list.currentIndex !== -1 && list.currentIndex < profileDataAccess.profileCount
            bgColor: colorScheme.positiveBackground
            onClicked: root.profileChosen(profileDataAccess.profile(list.currentIndex))
        }
    }
}
