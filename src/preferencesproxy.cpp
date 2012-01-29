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

int PreferencesProxy::requiredStrokesPerMinute() const
{
    return Preferences::requiredStrokesPerMinute();
}

void PreferencesProxy::setRequiredStrokesPerMinute(int strokesPerMinute)
{
    Preferences::setRequiredStrokesPerMinute(strokesPerMinute);
}

int PreferencesProxy::requiredAccuracy() const
{
    return Preferences::requiredAccuracy();
}

void PreferencesProxy::setRequiredAccuracy(int accuracy)
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
