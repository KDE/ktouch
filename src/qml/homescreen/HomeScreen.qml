/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *  SPDX-FileCopyrightText: 2015 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import ktouch 1.0

import "../common"

FocusScope {
    id: screen

    property KeyboardLayout selectedKeyboardLayout: KeyboardLayout {}
    property string activeKeyboardLayoutName
    signal lessonSelected(variant course, variant lesson, variant profile)

    Connections {
        target: profileDataAccess
        onProfileCountChanged: findCurrentProfile()
    }

    function start() {}
    function reset() {
        profileDataAccess.loadProfiles();
    }

    function findCurrentProfile() {
        profileComboBox.profile = null

        var lastProfileId = preferences.lastUsedProfileId

        for (var i = 0; i < profileDataAccess.profileCount; i++) {
            var profile = profileDataAccess.profile(i)
            if (profile.id === lastProfileId) {
                profileComboBox.profile = profile
                return;
            }
        }

        if (profileDataAccess.profileCount > 0) {
            profileComboBox.profile = profileDataAccess.profile(0)
            preferences.lastUsedProfileId = profileComboBox.profile.id
            preferences.writeConfig()
        }
    }

    function safeLastUsedProfile(profile) {
        preferences.lastUsedProfileId = profile.id
        preferences.writeConfig()
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        Item {
            id: navigationArea
            z: 2
            Layout.preferredWidth: 16 * Units.gridUnit
            Layout.fillHeight: true

            DropShadow {
                anchors.fill: navigationAreaLayout
                source: navigationAreaLayout
                radius: Units.largeSpacing
                samples: 2 * radius + 1
                horizontalOffset: 0
                verticalOffset: 0
            }

            ColumnLayout {
                id: navigationAreaLayout
                anchors.fill: parent
                spacing: 0

                ToolBar {
                    id: header

                    Layout.fillWidth: true
                    dimFactor: 1.3

                    ProfileComboBox {
                        anchors.fill: parent
                        id: profileComboBox
                        colorScheme: header.colorScheme
                        manageProfileButtonBgColor: header.colorScheme.toolbarBackground
                        onActivated: {
                            safeLastUsedProfile(profile)
                        }
                    }
                }

                CourseSelector {
                    id: courseSelector
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    profile: profileComboBox.profile
                    currentKeyboardLayoutName: screen.activeKeyboardLayoutName
                    onSelectedKeyboardLayoutChanged: {
                        dataAccess.loadKeyboardLayout(courseSelector.selectedKeyboardLayout, screen.selectedKeyboardLayout)
                    }
                }

            }
        }

        LessonSelector {
            Layout.fillHeight: true
            Layout.fillWidth: true
            profile: profileComboBox.profile
            selectedKeyboardLayout: screen.selectedKeyboardLayout
            activeKeyboardLayoutName: screen.activeKeyboardLayoutName
            dataIndexCourse: courseSelector.selectedCourse
            onLessonSelected: screen.lessonSelected(course, lesson, profileComboBox.profile)
            z: 1
            focus: true
        }
    }

    InitialProfileDialog {
        id: initialProfileForm
        visible: profileDataAccess.profileCount == 0
    }
}
