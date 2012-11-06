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

#include "preferencesproxy.h"

#include "preferences.h"

PreferencesProxy::PreferencesProxy(QObject* parent):
    QObject(parent)

{
    connect(Preferences::self(), SIGNAL(configChanged()), SIGNAL(configChanged()));
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
    Preferences::self()->writeConfig();
}
