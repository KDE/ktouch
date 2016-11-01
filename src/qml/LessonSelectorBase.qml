/*
 *  Copyright 2013  Sebastian Gottfried <sebastiangottfried@web.de>
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

Item {
    id: root

    property alias list: listContainer.data
    property alias previewArea: column
    property Lesson selectedLesson
    property bool selectedLessonLocked
    signal startButtonClicked()


    Row {
        anchors.fill: parent
        anchors.margins: 5
        spacing: 20

        Item {
            id: listContainer
            height: parent.height
            width: Math.round((parent.width - parent.spacing) / 2)

        }

        ColumnLayout {
            id: column
            width: parent.width - listContainer.width - parent.spacing
            height: parent.height
            spacing: 5

            LessonPreview {
                id: lessonPreview
                Layout.fillWidth: true
                Layout.fillHeight: true

                lesson: selectedLesson
            }

            LessonLockedNotice {
                anchors.centerIn: lessonPreview
                blurSource: lessonPreview
                opacity: selectedLessonLocked? 1: 0
            }

            Item {
                id: startButtonContainer
                Layout.fillWidth: true
                height: Math.round(1.5 * startButton.height)

                Button {
                    id: startButton
                    anchors.centerIn: parent
                    text: i18n("Start Training")
                    enabled: selectedLesson !== null && !selectedLessonLocked
                    iconName: "go-next-view"
                    onClicked: startButtonClicked()
                }
            }
        }
    }
}
