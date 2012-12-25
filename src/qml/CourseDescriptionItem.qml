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

Rectangle {
    id: root

    property string description
    property bool active: false

    height: active || content.opacity > 0? childrenRect.height: 0
    visible: height > 0
    color: theme.backgroundColor

    Behavior on height {
        NumberAnimation {
            duration: 150
            easing.type: Easing.InOutQuad
        }
    }

    onDescriptionChanged: {
        if (content.opacity === 0) {
            descriptionLabel.text = description
        }
        else {
            content.needsUpdate = true
        }
    }

    Item {
        id: content

        property bool needsUpdate: false

        width: parent.width
        height: descriptionLabel.height + 6
        opacity: root.active && !content.needsUpdate && root.height === root.childrenRect.height? 1: 0

        Behavior on opacity {
            NumberAnimation {
                duration: 150
            }
        }

        onOpacityChanged: {
            if (needsUpdate && opacity === 0) {
                descriptionLabel.text = root.description
                needsUpdate = false
            }
        }

        PlasmaComponents.Label {
            id: descriptionLabel
            anchors.centerIn: parent
            width: parent.width - 10
            font.pointSize: theme.smallestFont.pointSize
        }
    }

}
