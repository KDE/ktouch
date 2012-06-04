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


#ifndef COURSEEDITOR_H
#define COURSEEDITOR_H

#include <QWidget>
#include "ui_courseeditor.h"

class DataIndexCourse;
class Course;

class CourseEditor : public QWidget, private Ui::CourseEditor
{
public:
    explicit CourseEditor(QWidget* parent = 0);
    void openCourse(DataIndexCourse* dataIndexCourse);
private:
    void setIsReadOnly(bool readOnly);
    DataIndexCourse* m_dataIndexCourse;
    Course* m_course;
};

#endif // COURSEEDITOR_H
