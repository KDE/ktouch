/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
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
        }
        return null;
    }

    function keyLabel(position, key) {
        var keyChar = root.keyChar(position, key)
        return keyChar? keyChar.value: ""
    }

    color: key.state === "normal"? "#333": "#222"
    smooth: true
    font.pixelSize: referenceKey.height * Math.min(horizontalScaleFactor, verticalScaleFactor) / 3
    text: key.keyType() === "specialKey"?
        position === KeyChar.TopLeft? specialKeyLabel(key.type): "":
        keyLabel(position, key)
}
