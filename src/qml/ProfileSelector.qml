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
                                   i18n("Create new profile"):
                                   index < profileDataAccess.profileCount? profileDataAccess.profile(index).name: "nüscht"
                        label.font.italic: isNewButton
                        iconSource: isNewButton? "list-add": "user-identity"
                        onSelected: {
                            list.currentIndex = index
                            if (isNewButton) {
                                createNewProfile()
                            }
                            else {
                                selectProfile(index)
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
            }
        }

        PlasmaComponents.Button {
            id: selectButton
            anchors.horizontalCenter: parent.horizontalCenter
            iconSource: "go-next-view"
            text: i18n("Use selected profile")
            enabled: list.currentIndex !== -1 && list.currentIndex < profileDataAccess.profileCount
            onClicked: root.profileChosen(profileDataAccess.profile(list.currentIndex))
        }
    }
}
