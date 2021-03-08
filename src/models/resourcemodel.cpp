/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "resourcemodel.h"

#include <QIcon>

#include <KLocalizedString>
#include <KCategorizedSortFilterProxyModel>

ResourceModel::ResourceModel(QObject* parent) :
    QAbstractListModel(parent),
    m_dataIndex(0)
{
}

DataIndex* ResourceModel::dataIndex() const
{
    return m_dataIndex;
}

void ResourceModel::setDataIndex(DataIndex* dataIndex)
{
    if (dataIndex != m_dataIndex)
    {
        beginResetModel();

        if (m_dataIndex)
        {
            m_dataIndex->disconnect(this);
        }

        m_dataIndex = dataIndex;

        if (m_dataIndex)
        {
            connect(m_dataIndex, &DataIndex::courseAboutToBeAdded, this, &ResourceModel::onCourseAboutToBeAdded);
            connect(m_dataIndex, &DataIndex::courseAdded, this, &ResourceModel::onResourceAdded);
            connect(m_dataIndex, &DataIndex::coursesAboutToBeRemoved, this, &ResourceModel::onCoursesAboutToBeRemoved);
            connect(m_dataIndex, &DataIndex::coursesRemoved, this, &ResourceModel::onResourceRemoved);
            connect(m_dataIndex, &DataIndex::keyboardLayoutAboutToBeAdded, this, &ResourceModel::onKeyboardLayoutAboutToBeAdded);
            connect(m_dataIndex, &DataIndex::keyboardLayoutAdded, this, &ResourceModel::onResourceAdded);
            connect(m_dataIndex, &DataIndex::keyboardLayoutsAboutToBeRemoved, this, &ResourceModel::onKeyboardLayoutsAboutToBeRemoved);
            connect(m_dataIndex, &DataIndex::keyboardLayoutsRemoved, this, &ResourceModel::onResourceRemoved);
        }

        for (int i = 0; i < dataIndex->courseCount(); i++)
        {
            connectToCourse(dataIndex->course(i), i);
        }

        for (int i = 0; i < dataIndex->keyboardLayoutCount(); i++)
        {
            connectToKeyboardLayout(dataIndex->keyboardLayout(i), i);
        }

        updateMappings();

        endResetModel();

        emit dataIndexChanged();
    }
}

Qt::ItemFlags ResourceModel::flags(const QModelIndex& index) const
{
    return QAbstractListModel::flags(index);
}

QVariant ResourceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (!m_dataIndex)
        return QVariant();

    if (index.row() > m_dataIndex->courseCount() + m_dataIndex->keyboardLayoutCount())
        return QVariant();

    if (index.row() < m_dataIndex->courseCount())
        return courseData(index.row(), role);

    return keyboardLayoutData(index.row() - m_dataIndex->courseCount(), role);
}

QVariant ResourceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Vertical)
        return QVariant(section + 1);
    return QVariant(i18n("Title"));
}

int ResourceModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    if (!m_dataIndex)
        return 0;

    return m_dataIndex->courseCount() + m_dataIndex->keyboardLayoutCount();
}

QHash<int, QByteArray> ResourceModel::roleNames() const
{
    QHash<int, QByteArray> names = QAbstractItemModel::roleNames();
    names.insert(ResourceModel::KeyboardLayoutNameRole, "keyboardLayoutName");
    names.insert(ResourceModel::PathRole, "path");
    names.insert(ResourceModel::DataRole, "dataRole");
    return names;
}

void ResourceModel::onCourseAboutToBeAdded(DataIndexCourse* course, int index)
{
    connectToCourse(course, index);
    beginInsertRows(QModelIndex(), index, index);
}

void ResourceModel::onCoursesAboutToBeRemoved(int first, int last)
{
    beginRemoveRows(QModelIndex(), first, last);
}

void ResourceModel::onKeyboardLayoutAboutToBeAdded(DataIndexKeyboardLayout* keyboardLayout, int index)
{
    if (!m_dataIndex)
        return;

    const int offset = m_dataIndex->courseCount();
    connectToKeyboardLayout(keyboardLayout, index + offset);
    beginInsertRows(QModelIndex(), index + offset, index + offset);
}

void ResourceModel::onKeyboardLayoutsAboutToBeRemoved(int first, int last)
{
    if (!m_dataIndex)
        return;

    const int offset = m_dataIndex->courseCount();
    beginRemoveRows(QModelIndex(), first + offset, last + offset);
}

void ResourceModel::onResourceAdded()
{
    updateMappings();
    endInsertRows();
}

void ResourceModel::onResourceRemoved()
{
    updateMappings();
    endRemoveRows();
}

void ResourceModel::emitDataChanged(int row)
{
    const QModelIndex modelIndex = index(row);
    emit dataChanged(modelIndex, modelIndex);
}

QVariant ResourceModel::courseData(int row, int role) const
{
    if (!m_dataIndex)
        return QVariant();

    switch(role)
    {
    case KCategorizedSortFilterProxyModel::CategorySortRole:
        return QVariant(0);
    case KCategorizedSortFilterProxyModel::CategoryDisplayRole:
        return QVariant(i18n("Courses"));
    case Qt::DisplayRole:
        return QVariant(m_dataIndex->course(row)->title());
    case Qt::ToolTipRole:
        return QVariant(m_dataIndex->course(row)->description());
    case Qt::DecorationRole:
        return QVariant(resourceIcon(m_dataIndex->course(row)->source()));
    case ResourceModel::ResourceTypeRole:
        return QVariant(ResourceModel::CourseItem);
    case ResourceModel::DataRole:
        return QVariant::fromValue<QObject*>(m_dataIndex->course(row));
    case ResourceModel::KeyboardLayoutNameRole:
        return QVariant(m_dataIndex->course(row)->keyboardLayoutName());
    case ResourceModel::PathRole:
        return QVariant(m_dataIndex->course(row)->path());
    case ResourceModel::IndexRole:
        return QVariant(row);
    case ResourceModel::SourceRole:
        return QVariant(m_dataIndex->course(row)->source());
    default:
        return QVariant();
    }
}

QVariant ResourceModel::keyboardLayoutData(int row, int role) const
{
    if (!m_dataIndex)
        return QVariant();

    switch(role)
    {
    case KCategorizedSortFilterProxyModel::CategorySortRole:
        return QVariant(1);
    case KCategorizedSortFilterProxyModel::CategoryDisplayRole:
        return QVariant(i18n("Keyboard Layouts"));
    case Qt::DisplayRole:
        return QVariant(m_dataIndex->keyboardLayout(row)->title());
    case Qt::ToolTipRole:
        return QVariant(i18n("Name: %1", m_dataIndex->keyboardLayout(row)->name()));
    case Qt::DecorationRole:
        return QVariant(resourceIcon(m_dataIndex->keyboardLayout(row)->source()));
    case ResourceModel::ResourceTypeRole:
        return QVariant(ResourceModel::KeyboardLayoutItem);
    case ResourceModel::DataRole:
        return QVariant::fromValue<QObject*>(m_dataIndex->keyboardLayout(row));
    case ResourceModel::KeyboardLayoutNameRole:
        return QVariant(m_dataIndex->keyboardLayout(row)->name());
    case ResourceModel::PathRole:
        return QVariant(m_dataIndex->keyboardLayout(row)->path());
    case ResourceModel::IndexRole:
        return QVariant(row);
    case ResourceModel::SourceRole:
        return QVariant(m_dataIndex->keyboardLayout(row)->source());
    default:
        return QVariant();
    }
}

void ResourceModel::connectToCourse(DataIndexCourse* course, int index)
{
    disconnect(course, &DataIndexCourse::titleChanged, this, nullptr);
    disconnect(course, &DataIndexCourse::descriptionChanged, this, nullptr);
    disconnect(course, &DataIndexCourse::keyboardLayoutNameChanged, this, nullptr);
    disconnect(course, &DataIndexCourse::pathChanged, this, nullptr);
    disconnect(course, &DataIndexCourse::sourceChanged, this, nullptr);
    connect(course, &DataIndexCourse::titleChanged, this, [=] { emitDataChanged(index); });
    connect(course, &DataIndexCourse::descriptionChanged, this, [=] { emitDataChanged(index); });
    connect(course, &DataIndexCourse::keyboardLayoutNameChanged, this, [=] { emitDataChanged(index); });
    connect(course, &DataIndexCourse::pathChanged, this, [=] { emitDataChanged(index); });
    connect(course, &DataIndexCourse::sourceChanged, this, [=] { emitDataChanged(index); });
}

void ResourceModel::connectToKeyboardLayout(DataIndexKeyboardLayout *keyboardLayout, int index)
{
    disconnect(keyboardLayout, &DataIndexKeyboardLayout::titleChanged, this, nullptr);
    disconnect(keyboardLayout, &DataIndexKeyboardLayout::nameChanged, this, nullptr);
    disconnect(keyboardLayout, &DataIndexKeyboardLayout::pathChanged, this, nullptr);
    disconnect(keyboardLayout, &DataIndexKeyboardLayout::sourceChanged, this, nullptr);
    connect(keyboardLayout, &DataIndexKeyboardLayout::titleChanged, this, [=] { emitDataChanged(index); });
    connect(keyboardLayout, &DataIndexKeyboardLayout::nameChanged, this, [=] { emitDataChanged(index); });
    connect(keyboardLayout, &DataIndexKeyboardLayout::pathChanged, this, [=] { emitDataChanged(index); });
    connect(keyboardLayout, &DataIndexKeyboardLayout::sourceChanged, this, [=] { emitDataChanged(index); });
}

void ResourceModel::updateMappings()
{
    if (!m_dataIndex)
        return;

    for (int i = 0; i < m_dataIndex->courseCount(); i++)
    {
        connectToCourse(m_dataIndex->course(i), i);
    }

    const int offset = m_dataIndex->courseCount();

    for (int i = 0; i < m_dataIndex->keyboardLayoutCount(); i++)
    {
        connectToKeyboardLayout(m_dataIndex->keyboardLayout(i), i + offset);
    }
}

QIcon ResourceModel::resourceIcon(DataIndex::Source source) const
{
    static QIcon systemIcon = QIcon::fromTheme("computer");
    static QIcon userIcon = QIcon::fromTheme("user-identity");
    return source == DataIndex::BuiltInResource? systemIcon: userIcon;
}
