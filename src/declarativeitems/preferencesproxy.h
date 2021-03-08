/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
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
