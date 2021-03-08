/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "preferencesproxy.h"

#include "preferences.h"

PreferencesProxy::PreferencesProxy(QObject* parent):
    QObject(parent)

{
    connect(Preferences::self(), &KCoreConfigSkeleton::configChanged, this, &PreferencesProxy::configChanged);
}

bool PreferencesProxy::showKeyboard() const
{
    return Preferences::showKeyboard();
}

void PreferencesProxy::setShowKeyboard(bool showKeyboard)
{
    Preferences::setShowKeyboard(showKeyboard);
}

bool PreferencesProxy::showStatistics() const
{
    return Preferences::showStatistics();
}

void PreferencesProxy::setShowStatistics(bool showStatistics)
{
    Preferences::setShowStatistics(showStatistics);
}

bool PreferencesProxy::nextLineWithSpace() const
{
    return Preferences::nextLineWithSpace();
}

void PreferencesProxy::setNextLineWithSpace(bool nextLineWithSpace)
{
    Preferences::setNextLineWithSpace(nextLineWithSpace);
}

bool PreferencesProxy::nextLineWithReturn() const
{
    return Preferences::nextLineWithReturn();
}

void PreferencesProxy::setNextLineWithReturn(bool nextLineWithReturn)
{
    Preferences::setNextLineWithReturn(nextLineWithReturn);
}

int PreferencesProxy::requiredStrokesPerMinute() const
{
    return Preferences::requiredStrokesPerMinute();
}

void PreferencesProxy::setRequiredStrokesPerMinute(int strokesPerMinute)
{
    Preferences::setRequiredStrokesPerMinute(strokesPerMinute);
}

double PreferencesProxy::requiredAccuracy() const
{
    return Preferences::requiredAccuracy();
}

void PreferencesProxy::setRequiredAccuracy(double accuracy)
{
    Preferences::setRequiredStrokesPerMinute(accuracy);
}

int PreferencesProxy::lastUsedProfileId() const
{
    return Preferences::lastUsedProfileId();
}

void PreferencesProxy::setLastUsedProfileId(int profileId)
{
    Preferences::setLastUsedProfileId(profileId);
}

QColor PreferencesProxy::fingerColor(int index)
{
    return Preferences::fingerColor(index);
}

void PreferencesProxy::writeConfig()
{
    Preferences::self()->save();
}
