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
import org.kde.qtextracomponents 0.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import ktouch 1.0

FocusScope {

    id: root

    function save() {
        var profile = profileDataAccess.createProfile()
        profile.name = form.name
        profile.skillLevel = form.skillLevel
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
}
