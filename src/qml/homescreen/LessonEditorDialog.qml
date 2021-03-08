/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
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
        left: 2 * Units.gridUnit
        bottom: 2 * Units.gridUnit
        right: 2 * Units.gridUnit
        top: 2 * Units.gridUnit
    }

    width: parent.width - leftMargin - rightMargin
    height: parent.height - topMargin - bottomMargin

    padding: Units.gridUnit

    contentItem: GridLayout {

        columnSpacing: Units.gridUnit
        rowSpacing: Units.gridUnit

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
