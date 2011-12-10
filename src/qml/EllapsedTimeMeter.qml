import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore
import ktouch 1.0

Item
{
    id: meter
    width: childrenRect.width
    height: childrenRect.height

    Row {
        AnalogTimeMeter {
        }
        DigitalMeter {
            label: i18n("Ellapsed time")
            value: formatTime(stats.ellapsedTime)
            oldValue: "+/- 00:00.0"
        }
    }
}
