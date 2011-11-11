import QtQuick 1.0
import ktouch 1.0

Text
{
    property KeyCharModel keyCharModel

    color: key.state == "normal"? "#333": "#222"
    smooth: true
    font.pixelSize: referenceKeyModel.height * scaleFactor / 3
    text: keyCharModel && keyCharModel.value || ""
}
