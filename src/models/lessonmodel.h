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

#ifndef LESSONMODEL_H
#define LESSONMODEL_H

#include <QAbstractListModel>

class QSignalMapper;
class Course;
class Lesson;

class LessonModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Course* course READ course WRITE setCourse NOTIFY courseChanged)
public:
    explicit LessonModel(QObject* parent = 0);
    Course* course() const;
    void setCourse(Course* course);
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

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
    QSignalMapper* m_signalMapper;
};

#endif // LESSONMODEL_H
