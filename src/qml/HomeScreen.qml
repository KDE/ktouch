import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import ktouch 1.0

Item {
    id: screen

    property variant courses;
    signal lessonSelected(variant course, variant lesson)

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

        PlasmaComponents.ToolBar {
            visible: homeScreenAccordion.opacity > 0
            id: header
            width: parent.width
            tools: PlasmaComponents.ToolBarLayout {

                spacing: 5

                PlasmaComponents.ToolButton {
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
                }

                PlasmaComponents.ToolButton {
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

            HomeScreenAccordion {
                id: homeScreenAccordion
                opacity: 1 - initialProfileForm.opacity
                courses: screen.courses
                anchors.fill: parent
                anchors.margins: 5
                onLessonSelected: screen.lessonSelected(course, lesson)
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
