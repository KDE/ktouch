import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore

Item
{
    id: meter
    width: childrenRect.width
    height: childrenRect.height

    Row {
        AnalogMeter {
        }
        DigitalMeter {
            label: i18n("Strokes per second")
            value: stats.charactersPerMinute
            oldValue: "+/- 0"
        }
    }

}
