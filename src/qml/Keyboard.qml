import QtQuick 1.1
import ktouch 1.0

Item
{
    id: keyboard

    signal keyboardUpdate

    property KeyboardLayout keyboardLayout
    property int margin: 50
    property real aspectRatio: (keyboardLayout.width + margin) / (keyboardLayout.height + margin)
    property real scaleFactor: Math.min(width / (keyboardLayout.width + margin), height / (keyboardLayout.height + margin))

    function keyItems() {
        var items = []
        for (var i = 0; i < keys.count; i++)
            items.push(keys.itemAt(i))
        return items
    }

    function findKeyItem(keyChar) {
        for (var i = 0; i < keys.count; i++) {
            var key = keys.itemAt(i);
            if (key.match(keyChar))
                return key
        }
        return null

    }

    function handleKeyPress(event) {
        var key = findKeyItem(event)
        if (key)
            key.pressed = true
    }

    function handleKeyRelease(event) {
        var key = findKeyItem(event)
        if (key)
            key.pressed = false
    }

    Item {
        id: keyContainer

        width: childrenRect.width
        height: childrenRect.height
        anchors.centerIn: parent

        Repeater {
            id: keys
            model: keyboard.visible? keyboard.keyboardLayout.keyCount: 0

            onModelChanged: keyboard.keyboardUpdate()

            KeyItem {
                keyboardLayout: keyboard.keyboardLayout;
                keyIndex: index
            }
        }
    }
}
