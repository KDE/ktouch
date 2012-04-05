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

Item {
    id: root

    property Profile profile

    function update() {
        if (profile) {
            var isNewProfile = root.profile.id === -1
            infoModel.clear()
            profileForm.name = profile.name
            profileForm.skillLevel = profile.skillLevel
            profileForm.skillLevelSelectionEnabled = isNewProfile
            deleteConfirmationLabel.name = profile.name


            infoModel.append({
                                 title: i18n("%1:").arg(profile.name),
                                 value: profile.skillLevel === Profile.Beginner?
                                            i18n("Beginner typist"):
                                            i18n("Experienced typist")
                             });
            infoModel.append({
                                 title: i18n("Last training session:"),
                                 value: "TODO"
                             })
            infoModel.append({
                                 title: i18n("Total training time:"),
                                 value: "TODO"
                             })
            state = isNewProfile? "editor": "info"
        }

    }

    onProfileChanged: update()

    Item {
        id: infoContainer
        width: parent.width
        height: childrenRect.height
        anchors.centerIn: parent

        Column {
            width: parent.width
            height: childrenRect.height
            spacing: 40

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                width: 250
                height: 250
                color: theme.backgroundColor
                border.width: 1
            }

            InformationTable {
                model: ListModel {
                    id: infoModel
                }
            }

            Row {
                spacing: 10
                anchors.horizontalCenter: parent.horizontalCenter
                width: childrenRect.width
                height: childrenRect.height
                PlasmaComponents.ToolButton {
                    iconSource: "document-edit"
                    text: i18n("Edit")
                    onClicked: root.state = "editor"
                }
                PlasmaComponents.ToolButton {
                    iconSource: "edit-delete"
                    text: i18n("Delete")
                    enabled: profileDataAccess.profileCount > 1
                    onClicked: root.state = "deleteConfirmation"
                }
            }
        }
    }

    Item {
        id: editorContainer
        width: parent.width - 40
        height: childrenRect.height
        anchors.centerIn: parent
        ProfileForm {
            id: profileForm
            width: parent.width
            height: childrenRect.height
            showWelcomeLabel: false
            onDone: {
                root.profile.name = profileForm.name
                root.profile.skillLevel = profileForm.skillLevel
                if (root.profile.id === -1) {
                    profileDataAccess.addProfile(profile)
                }
                else {
                    profileDataAccess.updateProfile(profileDataAccess.indexOfProfile(root.profile))
                }
                root.update()
                root.state = "info"
            }
        }
    }

    Item {
        id: deleteConfirmationContainer
        width: parent.width - 40
        height: childrenRect.height
        anchors.centerIn: parent
        Column {
            width: parent.width
            height: childrenRect.height
            spacing: 15

            PlasmaComponents.Label {
                property string name
                id: deleteConfirmationLabel
                width: parent.width
                text: i18n("Do you really want to delete the profile \"<b>%1</b>\"?").arg(name)
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignHCenter
            }
            Row {
                spacing: 10
                anchors.horizontalCenter: parent.horizontalCenter
                width: childrenRect.width
                height: childrenRect.height
                PlasmaComponents.ToolButton {
                    iconSource: "edit-delete"
                    text: i18n("Delete")
                    onClicked: {
                        var index = profileDataAccess.indexOfProfile(root.profile)
                        root.profile = null
                        profileDataAccess.removeProfile(index)
                    }

                }
                PlasmaComponents.ToolButton {
                    text: i18n("Cancel")
                    onClicked: root.state = "info"
                }
            }
        }
    }

    states: [
        State {
            name: "info"
            PropertyChanges {
                target: infoContainer
                visible: true
            }
            PropertyChanges {
                target: editorContainer
                visible: false
            }
            PropertyChanges {
                target: deleteConfirmationContainer
                visible: false
            }
        },
        State {
            name: "editor"
            PropertyChanges {
                target: infoContainer
                visible: false
            }
            PropertyChanges {
                target: editorContainer
                visible: true
            }
            PropertyChanges {
                target: deleteConfirmationContainer
                visible: false
            }
        },
        State {
            name: "deleteConfirmation"
            PropertyChanges {
                target: infoContainer
                visible: false
            }
            PropertyChanges {
                target: editorContainer
                visible: false
            }
            PropertyChanges {
                target: deleteConfirmationContainer
                visible: true
            }
        }
    ]
}
