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

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import ktouch 1.0

FocusScope {

    id: root

    function save() {
        var profile = profileDataAccess.createProfile()
        profile.name = form.name
        profile.skillLevel = form.skillLevel
        profileDataAccess.addProfile(profile)
    }

    GroupBox  {
        id: frame
        anchors.centerIn: parent
        title: i18n("Welcome to Typewriting Trainer")
        width: form.width + 150
        height: form.height + 80

        Item {
            id: content
            anchors.fill: parent

            ProfileForm {
                id: form
                width: 400
                anchors.centerIn: parent
                doneButtonIconSource: "go-next-view"
                doneButtonText: i18n("Start Training")
                onDone: save()
            }
        }
    }
}
