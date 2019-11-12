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
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

import QtQuick 2.9
import ktouch 1.0

import "../common"

Collapsable {
    id: root

    property string description

    onDescriptionChanged: {
        if (contentItem.opacity === 0) {
            descriptionLabel.text = description
        }
        else {
            swapContent()
        }
    }

    onContentReadyForSwap: {
        descriptionLabel.text = description
    }

    Label {
        id: descriptionLabel
        leftPadding: 20
        rightPadding: 20
        topPadding: 10
        bottomPadding: 10
    }

}
