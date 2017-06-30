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
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import ktouch 1.0

import "../common"

FocusScope {
    id: screen

    property CategorizedResourceSortFilterProxyModel courseModel
    property KeyboardLayout keyboardLayout
    property string keyboardLayoutName
    signal lessonSelected(variant course, variant lesson, variant profile)

    QtObject {
        id: d

        property Profile profile
        property int profileCount: profileDataAccess.profileCount

        onProfileCountChanged: findCurrentProfile()
    }

    function start() {}
    function reset() {
        profileDataAccess.loadProfiles();
    }

    function findCurrentProfile() {
        d.profile = null

        var lastProfileId = preferences.lastUsedProfileId

        for (var i = 0; i < profileDataAccess.profileCount; i++) {
            var profile = profileDataAccess.profile(i)
            if (profile.id === lastProfileId) {
                d.profile = profile
                return;
            }
        }

        if (profileDataAccess.profileCount > 0) {
            d.profile = profileDataAccess.profile(0)
            preferences.lastUsedProfileId = d.profile.id
            preferences.writeConfig()
        }
    }

    function switchToProfile(profile) {
        d.profile = profile
        preferences.lastUsedProfileId = profile.id
        preferences.writeConfig()
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ToolBar {
            KColorScheme {
                id: toolbarColorScheme
                colorGroup: KColorScheme.Active
                colorSet: KColorScheme.Complementary
                property color toolbarBackground: Qt.darker(toolbarColorScheme.shade(toolbarColorScheme.hoverDecoration, KColorScheme.MidShade, toolbarColorScheme.contrast, -0.2), 1.3)
            }
            visible: courseSelector.opacity > 0
            id: header
            Layout.fillWidth: true
            height: 60

            background: Rectangle {
                color: toolbarColorScheme.toolbarBackground
            }


            RowLayout {
                anchors.fill: parent
                spacing: 5

                IconToolButton {
                    id: profileButton
                    icon: "user-identity"
                    text: d.profile !== null? d.profile.name: ""
                    color: toolbarColorScheme.normalText
                    backgroundColor: toolbarColorScheme.normalBackground
                    Layout.fillHeight: true
                    Layout.preferredWidth: 300
                    onClicked: {
                        if (checked) {
                            profileSelectorSheet.open()
                        }
                        else {
                            profileSelectorSheet.close()
                        }
                    }
                    checkable: true
                }

                Item {
                    Layout.fillWidth: true
                }

                IconToolButton {
                    id: configureButton
                    icon: "application-menu"
                    color: toolbarColorScheme.normalText
                    backgroundColor: toolbarColorScheme.normalBackground
                    Layout.fillHeight: true
                    Layout.preferredWidth: header.height
                    onClicked: {
                        var position = mapToItem(null, 0, height)
                        ktouch.showMenu(position.x, position.y)
                    }
                }
            }
        }

        Item {
            id: content
            Layout.fillWidth: true
            Layout.fillHeight: true

            RowLayout {
                anchors.fill: parent

                CourseSelector {
                    id: courseSelector
                    Layout.fillHeight: true
                    Layout.preferredWidth: 300
                    opacity: 1 - initialProfileForm.opacity
                    courseModel: screen.courseModel
                    profile: d.profile
                    keyboardLayout: screen.keyboardLayout
                    keyboardLayoutName: screen.keyboardLayoutName
                    onLessonSelected: screen.lessonSelected(course, lesson, d.profile)
                }

                Item {
                    id: filler
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
            }


            InitialProfileForm {
                id: initialProfileForm
                opacity: profileDataAccess.profileCount == 0? 1: 0
                anchors.fill: parent
                anchors.margins: 5

                Behavior on opacity {
                    NumberAnimation {
                        duration: screen.visible? 500: 0
                        easing.type: Easing.InOutCubic
                    }
                }
            }

            SheetDialog {
                id: profileSelectorSheet
                anchors.fill: parent
                onOpened: {
                    if (d.profile) {
                        var index = profileDataAccess.indexOfProfile(d.profile)
                        profileSelector.selectProfile(index)
                    }
                }
                onClosed: {
                    profileButton.checked = false;
                }
                content: ProfileSelector {
                    id: profileSelector
                    anchors.fill: parent
                    onProfileChosen: {
                        screen.switchToProfile(profile)
                        profileSelectorSheet.close()
                    }
                }
            }
        }
    }
}
