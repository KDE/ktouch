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

#include "errorsmodel.h"

#include "core/trainingstats.h"

bool lessThan(const QPair<QString,int>& left, const QPair<QString,int>& right)
{
    return left.second > right.second;
}

ErrorsModel::ErrorsModel(QObject* parent) :
    QAbstractTableModel(parent),
    m_trainingStats(0)
{
}

TrainingStats* ErrorsModel::trainingStats() const
{
    return m_trainingStats;
}

int ErrorsModel::maximumErrorCount() const
{
    if (m_errors.count() == 0)
        return 0;

    return m_errors.at(0).second;
}

void ErrorsModel::setTrainingStats(TrainingStats* trainingStats)
{
    if (trainingStats != m_trainingStats)
    {
        if (m_trainingStats)
        {
            m_trainingStats->disconnect(this);
        }

        m_trainingStats = trainingStats;

        if (m_trainingStats)
        {
            connect(m_trainingStats, SIGNAL(errorsChanged()), SLOT(buildErrorList()));
        }

        buildErrorList();
        emit trainingStatsChanged();
    }
}

QVariant ErrorsModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_errors.count())
        return QVariant();

    switch(role)
    {
    case Qt::DisplayRole:
        return QVariant(m_errors.at(index.row()).second);
    case Qt::ToolTipRole:
        return QVariant(m_errors.at(index.row()).first);
    default:
        return QVariant();
    }
}

int ErrorsModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return 1;
}

int ErrorsModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    if (!m_trainingStats)
        return 0;

    return m_trainingStats->errorMap().count();
}

QVariant ErrorsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Vertical)
        return QAbstractTableModel::headerData(section, orientation, role);

    switch (section)
    {
    case 0:
        return QVariant("errors");
    default:
        return QVariant();
    }
}

void ErrorsModel::buildErrorList()
{
    beginResetModel();

    m_errors.clear();

    if (!m_trainingStats)
    {
        endResetModel();
        return;
    }

    QMapIterator<QString, int> errorIterator(m_trainingStats->errorMap());

    while(errorIterator.hasNext())
    {
        errorIterator.next();
        m_errors.append(QPair<QString,int>(errorIterator.key(), errorIterator.value()));
    }

    qSort(m_errors.begin(), m_errors.end(), lessThan);

    emit maximumErrorCountChanged();

    endResetModel();
}

QString ErrorsModel::character(int row) const
{
    return m_errors.at(row).first;
}

int ErrorsModel::errors(int row) const
{
    return m_errors.at(row).second;
}
