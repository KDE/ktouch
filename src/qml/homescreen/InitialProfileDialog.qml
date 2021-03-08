/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *  SPDX-FileCopyrightText: 2015 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Layouts 1.3
import ktouch 1.0

import '../common'

PopupDialog {
    id: root
    modal: true
    title: i18n("Welcome to Typewriting Trainer")
    closePolicy: PopupDialog.NoAutoClose
    padding: Units.gridUnit

    function save() {
        var profile = profileDataAccess.createProfile()
        profile.name = form.name
        profile.skillLevel = form.skillLevel
        profileDataAccess.addProfile(profile)
        close()
    }

    contentItem: ProfileForm {
        id: form
        doneButtonIconSource: "go-next-view"
        doneButtonText: i18n("Start Training")
        onDone: save()
    }
}
