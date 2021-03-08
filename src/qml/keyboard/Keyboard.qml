/*
 *  SPDX-FileCopyrightText: 2013 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import ktouch 1.0

Item {
    id: keyboard

    signal keyboardUpdate

    property KeyboardLayout keyboardLayout
    property real aspectRatio: keyboardLayout.width / keyboardLayout.height
    property real horizontalScaleFactor: width / keyboardLayout.width
    property real verticalScaleFactor: height / keyboardLayout.height

    function keyItems() {
        var items = []

        for (var i = 0; i < keys.count; i++) {
            items.push(keys.itemAt(i))
        }

        return items
    }

    function findKeyItems(keyChar) {
        var matchingKeys = []

        for (var i = 0; i < keys.count; i++) {
            var key = keys.itemAt(i);
            if (key.match(keyChar))
            matchingKeys.push(key)
        }

        return matchingKeys
    }

    function findModifierKeyItem(modifierId) {
        for (var i = 0; i < keys.count; i++) {
            var key = keys.itemAt(i);
            if (key.key.keyType() === "specialKey" && key.key.modifierId === modifierId)
                return key
        }
        return null
    }

    function handleKeyPress(event) {
        var eventKeys = findKeyItems(event)

        for (var i = 0; i < eventKeys.length; i++) {
            eventKeys[i].pressed = true
        }
    }

    function handleKeyRelease(event) {
        var eventKeys = findKeyItems(event)

        for (var i = 0; i < eventKeys.length; i++) {
            eventKeys[i].pressed = false
        }
    }

    Item {
        id: keyContainer

        width: childrenRect.width
        height: childrenRect.height
        anchors.centerIn: parent

        Repeater {
            id: keys
            model: keyboard.visible && keyboardLayout.isValid? keyboard.keyboardLayout.keyCount: 0

            onModelChanged: keyboard.keyboardUpdate()

            KeyItem {
                keyboardLayout: keyboard.keyboardLayout;
                keyIndex: index
                horizontalScaleFactor: keyboard.horizontalScaleFactor
                verticalScaleFactor: keyboard.verticalScaleFactor
            }
        }
    }
}
