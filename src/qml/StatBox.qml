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

import QtQuick 1.1
import ktouch 1.0

Row {
    property TrainingStats stats
    property TrainingStats referenceStats

    height: childrenRect.height
    spacing: 10

    ElapsedTimeMeter {
        id: elapsedTimeMeter
        width: Math.floor((parent.width - 2 * parent.spacing) / 3)
        elapsedTime: stats.elapsedTime
        referenceElapsedTime: referenceStats.isValid? referenceStats.elapsedTime: stats.elapsedTime
    }

    CharactersPerMinuteMeter {
        id: charactersPerMinuteMeter
        width: elapsedTimeMeter.width
        charactersPerMinute: stats.charactersPerMinute
        referenceCharactersPerMinute: referenceStats.isValid? referenceStats.charactersPerMinute: stats.charactersPerMinute
    }

    AccuracyMeter {
        id: accuracyMeter
        width: parent.width - 2 * (elapsedTimeMeter.width + parent.spacing)
        accuracy: stats.accuracy
        referenceAccuracy: referenceStats.isValid? referenceStats.accuracy: stats.accuracy
    }
}
