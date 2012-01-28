import QtQuick 1.1
import org.kde.qtextracomponents 0.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import ktouch 1.0

FocusScope {

    id: root

    function save() {
        var profile = profileDataAccess.createProfile()
        profile.name = nameTextField.text
        profile.skillLevel = beginnerRadioBox.checked? Profile.Beginner: Profile.Advanced
        profileDataAccess.addProfile(profile)
    }

    PlasmaCore.FrameSvgItem  {
        id: frame
        anchors.centerIn: parent
        width: form.width + 150
        height: form.height + 80
        imagePath: "widgets/frame"
        prefix: "raised"

        Column {
            id: layout

            anchors {
                fill: parent
                topMargin: frame.margins.top
                rightMargin: frame.margins.right
                bottomMargin: frame.margins.bottom
                leftMargin: frame.margins.left
            }

            spacing: 0

            Item {
                id: header
                height: 20
                width: parent.width

                Row {
                    anchors.centerIn: parent
                    spacing: 6

                    QIconItem {
                        icon: QIcon("ktouch")
                        width: theme.smallIconSize
                        height: theme.smallIconSize
                    }

                    PlasmaComponents.Label {
                        id: titleLabel
                        text: i18n("Welcome to Typewriting Trainer")
                        height: paintedHeight
                    }

                }
            }

            PlasmaCore.SvgItem {
                id: line
                width: parent.width
                height: naturalSize.height
                elementId: "horizontal-line"
                svg: PlasmaCore.Svg {
                    imagePath: "widgets/line"
                }
            }

            Item {
                id: content
                width: parent.width
                height: parent.height - header.height - line.height

                Column {
                    id: form
                    width: 400
                    spacing: 15
                    anchors.centerIn: parent

                    PlasmaComponents.Label {
                        width: parent.width
                        text: i18n("Before you start training, please introduce yourself:")
                    }

                    PlasmaComponents.TextField {
                        id: nameTextField
                        width: parent.width
                        placeholderText: i18n("Name")
                    }

                    PlasmaComponents.RadioButton {
                        id: beginnerRadioBox
                        onClicked: {
                            if (checked) {
                                advancedRadioBox.checked = false;
                            }
                            else {
                                checked = true
                            }
                        }
                        width: parent.width
                        text: i18n("I have no or only very little experience in machine typing")
                    }

                    Row {
                        width: parent.width

                        Item {
                            width: 30
                            height: parent.height
                        }

                        PlasmaComponents.Label {
                            font.pointSize: theme.smallestFont.pointSize
                            height: paintedHeight
                            font.italic: true
                            text: i18n("Lessons are unlocked as your typing skills improve over time.")
                        }
                    }

                    PlasmaComponents.RadioButton {
                        id: advancedRadioBox
                        onClicked: {
                            if (checked) {
                                beginnerRadioBox.checked = false;
                            }
                            else {
                                checked = true
                            }
                        }
                        width: parent.width
                        text: i18n("I'm a expierenced machine typist and want to improve my skills")
                    }

                    Row {
                        width: parent.width

                        Item {
                            width: 30
                            height: parent.height
                        }

                        PlasmaComponents.Label {
                            font.pointSize: theme.smallestFont.pointSize
                            height: paintedHeight
                            font.italic: true
                            text: i18n("All lessons are unlocked immediately.")
                        }

                    }

                    PlasmaComponents.Button {
                        text: i18n("Start training")
                        enabled: nameTextField.text !== ""  && (beginnerRadioBox.checked || advancedRadioBox.checked)
                        iconSource: "go-next-view"
                        onClicked: save();
                    }
                }
            }
        }
    }
}
