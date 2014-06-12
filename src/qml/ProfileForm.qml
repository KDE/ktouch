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

Column {
    id: root

    property alias name: nameTextField.text
    property int skillLevel: 0

    property bool skillLevelSelectionEnabled: true
    property alias showWelcomeLabel: welcomeLabel.visible
    property alias doneButtonIconSource: doneBtn.iconSource
    property alias doneButtonText: doneBtn.text
    signal done()

    onSkillLevelChanged: {
        beginnerRadioButton.checked = skillLevel == Profile.Beginner
        advancedRadioButton.checked = skillLevel == Profile.Advanced
    }

    spacing: 15

    PlasmaComponents.Label {
        id: welcomeLabel
        width: parent.width
        text: i18n("Before you start training, please introduce yourself:")
    }

    PlasmaComponents.TextField {
        id: nameTextField
        width: parent.width
        placeholderText: i18n("Name")
    }

    DetailedRadioButton {
        id: beginnerRadioButton
        width: parent.width
        enabled: root.skillLevelSelectionEnabled
        label: i18n("I have no or only very little experience in machine typing")
        hint: i18n("Lessons are unlocked as your typing skills improve over time.")
        onCheckedChanged: {
            if (checked) {
                root.skillLevel = Profile.Beginner
                advancedRadioButton.checked = false
            }
        }
    }

    DetailedRadioButton {
        id: advancedRadioButton
        width: parent.width
        enabled: root.skillLevelSelectionEnabled
        label: i18n("I am an experienced machine typist and want to improve my skills")
        hint: i18n("All lessons are unlocked immediately.")
        onCheckedChanged: {
            if (checked) {
                root.skillLevel = Profile.Advanced
                beginnerRadioButton.checked = false
            }
        }
    }

    PlasmaComponents.Button {
        id: doneBtn
        anchors.horizontalCenter: parent.horizontalCenter
        text: i18n("Done")
        enabled: nameTextField.text !== "" && (beginnerRadioButton.checked || advancedRadioButton.checked)
        iconSource: "dialog-ok"
        onClicked: done()
    }
}
