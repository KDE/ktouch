/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Controls 2.2 as Controls
import ktouch 1.0

Controls.ScrollView {
    id: control

    Controls.ScrollBar.vertical: ScrollBar {
         parent: control
         x: control.mirrored ? 0 : control.width - width
         y: control.topPadding
         height: control.availableHeight
         active: control.ScrollBar.horizontal.active
         visible: size < 1
     }

     Controls.ScrollBar.horizontal: ScrollBar {
         parent: control
         x: control.leftPadding
         y: control.height - height
         width: control.availableWidth
         active: control.ScrollBar.vertical.active
         visible: size < 1
     }
}
