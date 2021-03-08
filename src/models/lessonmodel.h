/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef LESSONMODEL_H
#define LESSONMODEL_H

#include <QAbstractListModel>

class Course;
class Lesson;

class LessonModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Course* course READ course WRITE setCourse NOTIFY courseChanged)
    Q_ENUMS(AdditionalRoles)
public:
    enum AdditionalRoles {
        DataRole = Qt::UserRole + 1
    };
    explicit LessonModel(QObject* parent = nullptr);
    Course* course() const;
    void setCourse(Course* course);
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void courseChanged();
    void lessonChanged(int index);

private slots:
    void onLessonAboutToBeAdded(Lesson* lesson, int index);
    void onLessonAdded();
    void onLessonsAboutToBeRemoved(int first, int last);
    void onLessonsRemoved();
    void emitLessonChanged(int row);

private:
    void updateMappings();
    Course* m_course;
};

#endif // LESSONMODEL_H
