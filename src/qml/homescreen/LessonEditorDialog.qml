/*
 *  Copyright 2017  Sebastian Gottfried <sebastian.gottfried@posteo.de>
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
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

import QtQuick 2.9
import QtQuick.Layouts 1.3
import ktouch 1.0

import '../common'

PopupDialog {
    id: root

    property Lesson lesson: Lesson {}
    property KeyboardLayout keyboardLayout: KeyboardLayout {}
    property Profile profile: Profile {}

    onClosed: {
        profileDataAccess.storeCustomLesson(root.lesson, root.profile, root.keyboardLayout.name)
    }

    title: i18n("Edit lesson")

    margins: {
        left: 40
        bottom: 40
        right: 40
        top: 40
    }

    width: parent.width - leftMargin - rightMargin
    height: parent.height - topMargin - bottomMargin

    padding: titleLabel.font.pixelSize

    contentItem: GridLayout {

        columnSpacing: titleLabel.font.pixelSize
        rowSpacing: titleLabel.font.pixelSize

        Label {
            id: titleLabel
            text: i18n("Title:")
            Layout.row: 0
            Layout.column: 0
        }

        TextField {
            id: titleTextField
            text: root.lesson? root.lesson.title: ""
            onTextChanged: {
                if (root.lesson) {
                    root.lesson.title = text
                }
            }
            Layout.row: 0
            Layout.column: 1
            Layout.fillWidth: true
        }

        ScrollView {
            Layout.row: 1
            Layout.column: 0
            Layout.columnSpan: 2
            Layout.fillHeight: true
            Layout.fillWidth: true

            TextArea {
                id: lessonTextArea
                text: root.lesson? root.lesson.text: ""
                onTextChanged: {
                    if (root.lesson) {
                        root.lesson.text = text
                    }
                }
                placeholderText: i18n("Lesson text")
                font.family: "monospace"

                LessonTextHighlighter {
                    document: lessonTextArea.textDocument
                    allowedCharacters: root.visible? keyboardLayout.allCharacters(): ""
                }
            }
        }
    }

    footer: IconButton {
        text: i18n("Done")
        iconName: "dialog-ok"
        bgColor: colorScheme.positiveBackground
        onClicked: {
            root.close()
        }

    }
}
