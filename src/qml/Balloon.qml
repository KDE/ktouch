/*
 *   Copyright 2012 Marco Martin <mart@kde.org>
 *   Copyright 2015 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Library General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 2.4
import QtGraphicalEffects 1.0

Loader {
    id: root
    property Item visualParent
    property string status: 'closed'
    default property Item data
    active: status != 'closed'

    function open()
    {
        root.status = 'loading'
    }

    function close()
    {
        root.status = 'closing'
    }

    sourceComponent: Component {
        MouseArea {
            id: dismissArea
            anchors.fill: parent
            opacity: root.active && (root.status == 'open' || root.status =='opening')? 1 : 0
            layer.enabled: true
            layer.effect: DropShadow {
                anchors.fill: parent
                radius: 5
                samples: 11
            }

            Behavior on opacity {
                SequentialAnimation {
                    NumberAnimation {
                        duration: 250
                        easing.type: Easing.InOutQuad
                        properties: "opacity"
                    }
                    ScriptAction {
                        script: {
                            root.status = root.status == 'opening' ? 'open' : 'closed'
                        }

                    }
                }
            }

            SystemPalette {
                id: palette
                colorGroup: SystemPalette.Active
            }

            Rectangle {
                id: internal
                color: palette.alternateBase
                radius: 5

                property variant parentPos: root.visualParent? root.visualParent.mapToItem(dismissArea, 0, 0): Qt.point(0, 0)
                property bool under: root.visualParent ? internal.parentPos.y + root.visualParent.height + height < dismissArea.height : true

                //bindings won't work inside anchors definition
                onUnderChanged: {
                    if (under) {
                        balloonTip.anchors.top = undefined
                        balloonTip.anchors.bottom = balloonTip.parent.top
                    } else {
                        balloonTip.anchors.bottom = undefined
                        balloonTip.anchors.top = balloonTip.parent.bottom
                    }
                }

                property int preferedX: internal.parentPos.x - internal.width/2 + root.visualParent.width/2
                x: Math.round(Math.max(radius, Math.min(dismissArea.width - internal.width - radius, preferedX)))
                y: {
                    if (root.visualParent) {
                        if (under) {
                            Math.round(internal.parentPos.y + root.visualParent.height + balloonTip.height + radius)
                        } else {
                            Math.round(internal.parentPos.y - internal.height - balloonTip.height - radius)
                        }
                    } else {
                        Math.round(dismissArea.height/2 - internal.height/2)
                    }
                }
                width: contentItem.width + 2 * internal.radius
                height: contentItem.height + 2 * internal.radius

                Rectangle {
                    id: balloonTip
                    color: internal.color
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        horizontalCenterOffset: internal.preferedX - internal.x
                        top: parent.bottom
                    }
                    width: 10
                    height: 10
                    visible: false
                }

                Image {
                    id: balloonTipMask
                    anchors.fill: balloonTip
                    visible: false
                    source: utils.findImage("balloontip.svgz")
                    sourceSize: Qt.size(width, height)
               }

                OpacityMask {
                    anchors.fill: balloonTip
                    visible: root.visualParent != null
                    source: balloonTip
                    maskSource: balloonTipMask
                    rotation: internal.under? 0: 180
                }

                Item {
                    id: contentItem
                    x: internal.radius
                    y: internal.radius
                    width: childrenRect.width
                    height: childrenRect.height + 2
                    data: root.data
                }
            }

            onClicked: {
                root.close()
            }

            Component.onCompleted: {
                var candidate = root
                while (candidate.parent.parent) {
                    candidate = candidate.parent
                }
                if (candidate) {
                    dismissArea.parent = candidate
                }
                root.status = 'opening'
            }

        }

    }
}
