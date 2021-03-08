/*
 * SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
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
    if (m_errors.isEmpty())
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
            connect(m_trainingStats, &TrainingStats::errorsChanged, this, &ErrorsModel::buildErrorList);
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

    std::sort(m_errors.begin(), m_errors.end(), lessThan);

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
