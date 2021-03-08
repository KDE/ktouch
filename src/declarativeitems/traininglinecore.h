/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef TRAININGLINECORE_H
#define TRAININGLINECORE_H

#include <QQuickItem>

#include <QPointer>

class TrainingStats;

class TrainingLineCore : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(QQuickItem* cursorItem READ cursorItem WRITE setCursorItem NOTIFY cursorItemChanged)
    Q_PROPERTY(TrainingStats* trainingStats READ trainingStats WRITE setTrainingStats NOTIFY trainingStatsChanged)
    Q_PROPERTY(QString referenceLine READ referenceLine WRITE setReferenceLine NOTIFY referenceLineChanged)
    Q_PROPERTY(QString actualLine READ actualLine NOTIFY actualLineChanged)
    Q_PROPERTY(QString preeditString READ preeditString NOTIFY preeditStringChanged)
    Q_PROPERTY(bool isCorrect READ isCorrect NOTIFY actualLineChanged)
    Q_PROPERTY(QString nextCharacter READ nextCharacter NOTIFY actualLineChanged)
    Q_PROPERTY(int hintKey READ hintKey NOTIFY hintKeyChanged)
public:
    explicit TrainingLineCore(QQuickItem* parent = 0);
    bool active() const;
    void setActive(bool active);
    QQuickItem* cursorItem() const;
    void setCursorItem(QQuickItem* cursorItem);
    TrainingStats* trainingStats() const;
    void setTrainingStats(TrainingStats* trainingStats);
    QString referenceLine() const;
    void setReferenceLine(const QString& referenceLine);
    QString actualLine() const;
    QString preeditString() const;
    bool isCorrect() const;
    QString nextCharacter() const;
    int hintKey() const;
public slots:
    void reset();
signals:
    void activeChanged();
    void cursorItemChanged();
    void trainingStatsChanged();
    void referenceLineChanged();
    void actualLineChanged();
    void preeditStringChanged();
    void hintKeyChanged();
    void done();
protected:
    void keyPressEvent(QKeyEvent* event) override;
    void inputMethodEvent(QInputMethodEvent* event) override;
    QVariant inputMethodQuery(Qt::InputMethodQuery query) const override;
private:
    void add(const QString& text);
    void backspace();
    void deleteStartOfWord();
    void clearActualLine();
    void giveKeyHint(int key);
    void clearKeyHint();
    bool m_active;
    TrainingStats* m_trainingStats;
    QString m_referenceLine;
    QString m_actualLine;
    QString m_preeditString;
    int m_hintKey;
    int m_keyHintOccurrenceCount;
    QPointer<QQuickItem> m_cursorItem;
};

#endif // TRAININGLINECORE_H
