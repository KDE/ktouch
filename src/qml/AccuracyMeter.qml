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
            label: i18n("Hit ratio")
            value: formatAccuracy(stats.accuracy)
            oldValue: "+/- 0.0%"
        }
    }

}
