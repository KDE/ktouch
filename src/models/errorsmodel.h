/*
 * Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
    explicit ErrorsModel(QObject* parent = 0);
    TrainingStats* trainingStats() const;
    void setTrainingStats(TrainingStats* trainingStats);
    int maximumErrorCount() const;
    QVariant data(const QModelIndex& index, int role) const;
    int columnCount(const QModelIndex& parent) const;
    int rowCount(const QModelIndex& parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
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
