/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef DATAINDEX_H
#define DATAINDEX_H

#include "resource.h"

#include <QString>
#include <QList>

#include "coursebase.h"
#include "keyboardlayoutbase.h"

class DataIndexCourse;
class DataIndexKeyboardLayout;

class DataIndex : public Resource
{
    Q_OBJECT
    Q_PROPERTY(int courseCount READ courseCount NOTIFY courseCountChanged)
    Q_PROPERTY(int keyboardLayoutCount READ keyboardLayoutCount NOTIFY keyboardLayoutCountChanged)
    Q_ENUMS(Source)

public:
    enum Source {
        BuiltInResource,
        UserResource
    };
    explicit DataIndex(QObject* parent = 0);
    int courseCount() const;
    Q_INVOKABLE DataIndexCourse* course(int index) const;
    Q_INVOKABLE void addCourse(DataIndexCourse* course);
    Q_INVOKABLE void removeCourse(int index);
    Q_INVOKABLE void clearCourses();
    int keyboardLayoutCount() const;
    Q_INVOKABLE DataIndexKeyboardLayout* keyboardLayout(int index) const;
    Q_INVOKABLE void addKeyboardLayout(DataIndexKeyboardLayout* keyboardLayout);
    Q_INVOKABLE void removeKeyboardLayout(int index);
    Q_INVOKABLE void clearKeyboardLayouts();

signals:
    void courseCountChanged();
    void keyboardLayoutCountChanged();

    void courseAboutToBeAdded(DataIndexCourse* course, int index);
    void courseAdded();
    void coursesAboutToBeRemoved(int first, int last);
    void coursesRemoved();

    void keyboardLayoutAboutToBeAdded(DataIndexKeyboardLayout* keyboardLayout, int index);
    void keyboardLayoutAdded();
    void keyboardLayoutsAboutToBeRemoved(int first, int last);
    void keyboardLayoutsRemoved();

private:
    QList<DataIndexCourse*> m_courses;
    QList<DataIndexKeyboardLayout*> m_keyboardLayouts;
};

class DataIndexCourse: public CourseBase
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(DataIndex::Source source READ source WRITE setSource NOTIFY sourceChanged)

public:
    explicit DataIndexCourse(QObject* parent = 0);
    QString path() const;
    void setPath(const QString& path);
    DataIndex::Source source() const;
    void setSource(DataIndex::Source source);

signals:
    void pathChanged();
    void sourceChanged();

private:
    QString m_path;
    DataIndex::Source m_source;
};

class DataIndexKeyboardLayout: public KeyboardLayoutBase
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(DataIndex::Source source READ source WRITE setSource NOTIFY sourceChanged)

public:
    explicit DataIndexKeyboardLayout(QObject* parent = 0);

    QString path() const;
    void setPath(const QString& path);
    DataIndex::Source source() const;
    void setSource(DataIndex::Source source);

signals:
    void pathChanged();
    void sourceChanged();

private:
    QString m_path;
    DataIndex::Source m_source;
};

#endif // DATAINDEX_H
