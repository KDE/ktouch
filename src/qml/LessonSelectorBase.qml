/*
 *  Copyright 2013  Sebastian Gottfried <sebastiangottfried@web.de>
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

        Column {
            id: column
            width: parent.width - listContainer.width - parent.spacing
            height: parent.height
            spacing: 5

            LessonPreview {
                id: lessonPreview
                width: parent.width
                height: parent.height - startButtonContainer.height
                lesson: selectedLesson

                LessonLockedNotice {
                    anchors.centerIn: parent
                    opacity: selectedLessonLocked? 1: 0
                }
            }

            Item {
                id: startButtonContainer
                width: parent.width
                height: Math.round(1.5 * startButton.height)

                PlasmaComponents.Button {
                    id: startButton
                    anchors.centerIn: parent
                    text: i18n("Start Training")
                    enabled: selectedLesson !== null && !selectedLessonLocked
                    iconSource: "go-next-view"
                    onClicked: startButtonClicked()
                }
            }
        }
    }
}
