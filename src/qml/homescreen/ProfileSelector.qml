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

    ColumnLayout {
        anchors.fill: parent
        anchors.bottomMargin: 10
        spacing: 10

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 10

            Item {
                id: listContainer
                Layout.fillWidth: true
                Layout.fillHeight: true

                ScrollView {
                    anchors.fill: parent
                    ListView {
                        id: list
                        anchors.fill: parent
                        anchors.margins: 3
                        spacing: 3
                        model: profileDataAccess.profileCount + 1
                        clip: true
                        delegate: ListItem {
                            property bool isNewButton: index >= profileDataAccess.profileCount
                            width: list.width
                            title: isNewButton?
                                    i18n("Create New Profile"):
                                    index < profileDataAccess.profileCount? profileDataAccess.profile(index).name: null
                            label.font.italic: isNewButton
                            iconSource: isNewButton? "list-add": "user-identity"
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

        Button {
            id: selectButton
            anchors.horizontalCenter: parent.horizontalCenter
            iconName: "go-next-view"
            text: i18n("Use Selected Profile")
            enabled: list.currentIndex !== -1 && list.currentIndex < profileDataAccess.profileCount
            onClicked: root.profileChosen(profileDataAccess.profile(list.currentIndex))
        }
    }
}
