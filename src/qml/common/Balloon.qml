/*
 *   SPDX-FileCopyrightText: 2012 Marco Martin <mart@kde.org>
 *   SPDX-FileCopyrightText: 2015 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *   SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.9
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
                radius: Units.smallSpacing
                samples: 2 * radius + 1
            }

            Behavior on opacity {
                SequentialAnimation {
                    NumberAnimation {
                        duration: Units.shortDuration
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
                radius: Units.smallSpacing

                property variant parentPos: root.visualParent? root.visualParent.mapToItem(null, 0, 0): Qt.point(0, 0)
                property bool under: root.visualParent ? internal.parentPos.y + root.visualParent.height + height < dismissArea.height : true

                // bindings don't work for anchor definition
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
                    width: Math.floor(Units.gridUnit / 2)
                    height: width
                    visible: false
                }

                Image {
                    id: balloonTipMask
                    anchors.fill: balloonTip
                    visible: false
                    source: "qrc:///ktouch/images/balloontip.svgz"
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
