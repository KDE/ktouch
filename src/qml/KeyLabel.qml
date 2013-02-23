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

import QtQuick 1.0
import ktouch 1.0

Text {
    id: root

    property AbstractKey key
    property variant position

    function specialKeyLabel(type) {
        switch (key.type) {
        case SpecialKey.Other:
            return key.label
        case SpecialKey.Tab:
            return "\u21B9"
        case SpecialKey.Capslock:
            return "\u21E9"
        case SpecialKey.Shift:
            return "\u21E7"
        case SpecialKey.Backspace:
            return  "\u2190"
        case SpecialKey.Return:
            return  "\u21B5"
        case SpecialKey.Space:
            return "";
        }
    }

    function keyChar(position, key) {
        for (var i = 0; i < key.keyCharCount; i++) {
            var keyChar = key.keyChar(i);
            if (position === keyChar.position) {
                return keyChar
            }
            else if (position === keyChar.position) {
                return keyChar
            }
            else if (position === keyChar.position) {
                return keyChar
            }
            else if (position === keyChar.position) {
                return keyChar
            }
        }
        return null;
    }

    function keyLabel(position, key) {
        var keyChar = root.keyChar(position, key)
        return keyChar? String.fromCharCode(keyChar.value): ""
    }

    color: key.state == "normal"? "#333": "#222"
    smooth: true
    font.pixelSize: referenceKey.height * Math.min(horizontalScaleFactor, verticalScaleFactor) / 3
    text: key.keyType() == "specialKey"?
        position == KeyChar.TopLeft? specialKeyLabel(key.type): "":
        keyLabel(position, key)
}
