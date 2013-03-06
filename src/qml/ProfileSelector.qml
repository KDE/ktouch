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
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import ktouch 1.0

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

    Column {
        anchors.fill: parent
        spacing: 10

        Row {
            height: parent.height - selectButton.height - parent.spacing
            width: parent.width
            spacing: 10

            Item {
                id: listContainer
                height: parent.height
                width: Math.round((parent.width - parent.spacing) / 2)

                ListView {
                    id: list
                    anchors.fill: parent
                    model: profileDataAccess.profileCount + 1
                    clip: true
                    delegate: ListItem {
                        property bool isNewButton: index >= profileDataAccess.profileCount
                        width: list.width - scrollBar.width
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

                PlasmaComponents.ScrollBar {
                    id: scrollBar
                    flickableItem: list
                }
            }

            PlasmaCore.SvgItem {
                id: line
                width: naturalSize.width
                height: parent.height
                elementId: "vertical-line"
                svg: PlasmaCore.Svg {
                    imagePath: "widgets/line"
                }
            }

            ProfileDetailsItem {
                id: profileForm
                width: parent.width - listContainer.width - line.width - 2 * parent.spacing
                height: parent.height

                onDeletionRequest: {
                    var index = profileDataAccess.indexOfProfile(profileForm.profile)
                    profileForm.profile = null
                    profileDataAccess.removeProfile(index)
                    selectProfile(Math.max(0, list.currentIndex - 1))
                }
            }
        }

        PlasmaComponents.Button {
            id: selectButton
            anchors.horizontalCenter: parent.horizontalCenter
            iconSource: "go-next-view"
            text: i18n("Use Selected Profile")
            enabled: list.currentIndex !== -1 && list.currentIndex < profileDataAccess.profileCount
            onClicked: root.profileChosen(profileDataAccess.profile(list.currentIndex))
        }
    }
}
