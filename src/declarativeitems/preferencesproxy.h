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

#ifndef PREFERENCESPROXY_H
#define PREFERENCESPROXY_H

#include <QObject>

#include <QColor>

class PreferencesProxy : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool showKeyboard READ showKeyboard WRITE setShowKeyboard NOTIFY configChanged)
    Q_PROPERTY(bool showStatistics READ showStatistics WRITE setShowStatistics NOTIFY configChanged)
    Q_PROPERTY(bool nextLineWithSpace READ nextLineWithSpace WRITE setNextLineWithSpace NOTIFY configChanged)
    Q_PROPERTY(bool nextLineWithReturn READ nextLineWithReturn WRITE setNextLineWithReturn NOTIFY configChanged)
    Q_PROPERTY(int requiredStrokesPerMinute READ requiredStrokesPerMinute WRITE setRequiredStrokesPerMinute NOTIFY configChanged)
    Q_PROPERTY(double requiredAccuracy READ requiredAccuracy WRITE setRequiredAccuracy NOTIFY configChanged)
    Q_PROPERTY(int lastUsedProfileId READ lastUsedProfileId WRITE setLastUsedProfileId NOTIFY configChanged)

public:
    explicit PreferencesProxy(QObject* parent = 0);
    bool showKeyboard() const;
    void setShowKeyboard(bool showKeyboard);
    bool showStatistics() const;
    void setShowStatistics(bool showStatistics);
    bool nextLineWithSpace() const;
    void setNextLineWithSpace(bool nextLineWithSpace);
    bool nextLineWithReturn() const;
    void setNextLineWithReturn(bool nextLineWithReturn);
    int requiredStrokesPerMinute() const;
    void setRequiredStrokesPerMinute(int strokesPerMinute);
    double requiredAccuracy() const;
    void setRequiredAccuracy(double accuracy);
    int lastUsedProfileId() const;
    void setLastUsedProfileId(int profileId);
    Q_INVOKABLE QColor fingerColor(int index);
    Q_INVOKABLE void writeConfig();

signals:
    void configChanged();
};

#endif // PREFERENCESPROXY_H
