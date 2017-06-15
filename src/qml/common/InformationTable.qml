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

Item {
    property alias model: repeator.model
    width: parent.width
    height: childrenRect.height

    Column {
        id: column
        spacing: 3
        width: parent.width
        height: childrenRect.height

        Repeater {
            id: repeator
            Row {
                spacing: 5
                height: Math.max(titleLabel.height, valueLabel.height)
                width: column.width
                Label {
                    id: titleLabel
                    width: Math.round((parent.width - parent.spacing) / 2)
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignTop
                    color: "#888"
                    text: model.modelData.title
                    wrapMode: Text.Wrap
                    height: Math.max(paintedHeight, valueLabel.paintedHeight)
                }
                Label {
                    id: valueLabel
                    width: parent.width - titleLabel.width - parent.spacing
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignBottom
                    text: model.modelData.text
                    wrapMode: Text.Wrap
                    height: Math.max(paintedHeight, titleLabel.paintedHeight)
                }
            }
        }
    }
}
