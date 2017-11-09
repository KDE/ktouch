/*
 *  Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
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

import QtQuick 2.6
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import ktouch 1.0

import '../common'

PopupDialog {
    id: root
    modal: true
    title: i18n("Welcome to Typewriting Trainer")
    closePolicy: Popup.NoAutoClose
    padding: 20

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
