import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore

Item
{
    id: meter

    property int charactersPerMinute: 0

    width: childrenRect.width
    height: childrenRect.height

    Row {
        AnalogCharactersPerMinuteMeter {
            charactersPerMinute: meter.charactersPerMinute
        }
        DigitalMeter {
            label: i18n("Strokes per minute")
            value: meter.charactersPerMinute
            oldValue: "+/- 0"
        }
    }

}
