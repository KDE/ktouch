/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *  SPDX-FileCopyrightText: 2015 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Layouts 1.3
import ktouch 1.0

RowLayout {
    property TrainingStats stats
    property TrainingStats referenceStats

    height: childrenRect.height
    spacing: 10

    ElapsedTimeMeter {
        id: elapsedTimeMeter
        Layout.fillWidth: true
        elapsedTime: stats.elapsedTime
        referenceElapsedTime: referenceStats.isValid? referenceStats.elapsedTime: stats.elapsedTime
    }

    CharactersPerMinuteMeter {
        id: charactersPerMinuteMeter
        Layout.fillWidth: true
        charactersPerMinute: stats.charactersPerMinute
        referenceCharactersPerMinute: referenceStats.isValid? referenceStats.charactersPerMinute: stats.charactersPerMinute
    }

    AccuracyMeter {
        id: accuracyMeter
        Layout.fillWidth: true
        accuracy: stats.accuracy
        referenceAccuracy: referenceStats.isValid? referenceStats.accuracy: stats.accuracy
    }
}
