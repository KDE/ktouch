/*
 * SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef ERRORSMODEL_H
#define ERRORSMODEL_H

#include <QAbstractTableModel>

class TrainingStats;

class ErrorsModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(TrainingStats* trainingStats READ trainingStats WRITE setTrainingStats NOTIFY trainingStatsChanged)
    Q_PROPERTY(int maximumErrorCount READ maximumErrorCount NOTIFY maximumErrorCountChanged)
public:
    explicit ErrorsModel(QObject* parent = nullptr);
    TrainingStats* trainingStats() const;
    void setTrainingStats(TrainingStats* trainingStats);
    int maximumErrorCount() const;
    QVariant data(const QModelIndex& index, int role) const override;
    int columnCount(const QModelIndex& parent) const override;
    int rowCount(const QModelIndex& parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Q_INVOKABLE QString character(int row) const;
    Q_INVOKABLE int errors(int row) const;
signals:
    void trainingStatsChanged();
    void maximumErrorCountChanged();
private slots:
    void buildErrorList();
private:
    TrainingStats* m_trainingStats;
    QList<QPair<QString, int> > m_errors;
};

#endif // ERRORSMODEL_H
