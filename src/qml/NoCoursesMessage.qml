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

Item {
    id: item

    PlasmaCore.FrameSvgItem  {
        id: frame
        anchors.fill: parent
        anchors.margins: 40
        imagePath: "widgets/frame"
        prefix: "raised"

        Column {
            id: layout
            anchors {
                fill: parent
                topMargin: frame.margins.top + spacing
                rightMargin: frame.margins.right + spacing
                bottomMargin: frame.margins.bottom + spacing
                leftMargin: frame.margins.left + spacing
            }

            spacing: 20

            Rectangle {
                width: parent.width
                color: "#eee4be"
                height: 80
                radius: 15
                PlasmaComponents.Label {
                    anchors.centerIn: parent
                    text: i18n("No Courses Available")
                    height: paintedHeight
                    font.pointSize: 1.5 * theme.defaultFont.pointSize
                }
            }

            QIconItem {
                id: icon
                anchors.horizontalCenter: parent.horizontalCenter
                icon: QIcon("face-sad")
                width: theme.enormousIconSize
                height: theme.enormousIconSize
            }

            PlasmaComponents.Label {
                width: parent.width
                text: i18n("Typewriting Trainer has no courses for your keyboard layout. You may create your own course with the keyboard layout and course editor.")
            }

            PlasmaComponents.Button {
                text: i18n("Launch Editor")
                onClicked: showResourceEditor()
            }
        }
    }
}
