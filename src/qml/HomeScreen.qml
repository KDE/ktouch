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

    Column {
        anchors.fill: parent
        spacing: header.margins.bottom / 2

        PlasmaComponents.ToolBar {
            visible: homeScreenAccordion.opacity > 0
            id: header
            width: parent.width
            tools: Row {
                anchors.leftMargin: 3
                anchors.rightMargin: 3
                spacing: 5

                PlasmaComponents.ToolButton {
                    id: profileButton
                    iconSource: "user-identity"
                    text: d.profile !== null? d.profile.name: ""
                    onClicked: {
                        if (profileSelectorSheet.isOpen()) {
                            profileSelectorSheet.close()
                        }
                        else {
                            profileSelectorSheet.open()
                        }
                    }
                    checked: profileSelectorSheet.isOpen()
                    width: minimumWidth
                }

                Item {
                    height: parent.height
                    width: parent.width - profileButton.width - configureButton.width - (parent.children.length - 1) * parent.spacing
                }

                PlasmaComponents.ToolButton {
                    id: configureButton
                    iconSource: "configure"
                    onClicked: {
                        var position = mapToItem(null, 0, height)
                        showMenu(position.x, position.y)
                    }
                }
            }
        }

        Item {
            id: content
            width: parent.width
            height: parent.height - header.height

            CourseSelector {
                id: homeScreenAccordion
                opacity: 1 - initialProfileForm.opacity
                courseModel: screen.courseModel
                profile: d.profile
                keyboardLayout: screen.keyboardLayout
                keyboardLayoutName: screen.keyboardLayoutName
                anchors.fill: parent
                onLessonSelected: screen.lessonSelected(course, lesson, d.profile)
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
                onOpended: {
                    if (d.profile) {
                        var index = profileDataAccess.indexOfProfile(d.profile)
                        profileSelector.selectProfile(index)
                    }
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
