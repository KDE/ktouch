#ifndef PREFERENCESPROXY_H
#define PREFERENCESPROXY_H

#include <QObject>

#include <QColor>

class PreferencesProxy : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool showKeyboard READ showKeyboard WRITE setShowKeyboard NOTIFY configChanged)
    Q_PROPERTY(int requiredStrokesPerMinute READ requiredStrokesPerMinute WRITE setRequiredStrokesPerMinute NOTIFY configChanged)
    Q_PROPERTY(int requiredAccuracy READ requiredAccuracy WRITE setRequiredAccuracy NOTIFY configChanged)

public:
    explicit PreferencesProxy(QObject* parent = 0);
    bool showKeyboard() const;
    void setShowKeyboard(bool showKeyboard);
    int requiredStrokesPerMinute() const;
    void setRequiredStrokesPerMinute(int strokesPerMinute);
    int requiredAccuracy() const;
    void setRequiredAccuracy(int accuracy);
    Q_INVOKABLE QColor fingerColor(int index);
    Q_INVOKABLE void writeConfig();

signals:
    void configChanged();
};

#endif // PREFERENCESPROXY_H
