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

import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1
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
        spacing: 0

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                id: listContainer
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: listColorScheme.normalBackground

                ScrollView {
                    anchors.fill: parent
                    ListView {
                        id: list
                        anchors.fill: parent
                        model: profileDataAccess.profileCount + 1
                        clip: true
                        delegate: ListItem {
                            property bool isNewButton: index >= profileDataAccess.profileCount
                            width: list.width
                            text: isNewButton?
                                    i18n("Create New Profile"):
                                    index < profileDataAccess.profileCount? profileDataAccess.profile(index).name: null
                            label.font.italic: isNewButton
                            icon: isNewButton? "list-add": "user-identity"
                            highlighted: ListView.isCurrentItem
                            onClicked: {
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
            icon: "go-next-view"
            text: i18n("Use Selected Profile")
            enabled: list.currentIndex !== -1 && list.currentIndex < profileDataAccess.profileCount
            bgColor: colorScheme.positiveBackground
            onClicked: root.profileChosen(profileDataAccess.profile(list.currentIndex))
        }
    }
}
