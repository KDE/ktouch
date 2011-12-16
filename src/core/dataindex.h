#ifndef DATAINDEX_H
#define DATAINDEX_H

#include <QObject>

#include <QString>
#include <QList>

#include "coursebase.h"
#include "keyboardlayoutbase.h"

class DataIndexCourse: public CourseBase
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)

public:
    explicit DataIndexCourse(QObject* parent = 0);

    QString path() const
    {
        return m_path;
    }

    void setPath(const QString& path)
    {
        if (path != m_path)
        {
            m_path = path;
            emit pathChanged();
        }
    }

signals:
    void pathChanged();

private:
    QString m_path;

};

class DataIndexKeyboardLayout: public KeyboardLayoutBase
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)

public:
    explicit DataIndexKeyboardLayout(QObject* parent = 0);

    QString path() const
    {
        return m_path;
    }

    void setPath(const QString& path)
    {
        if (path != m_path)
        {
            m_path = path;
            emit pathChanged();
        }
    }

signals:
    void pathChanged();

private:
    QString m_path;

};

class DataIndex : public QObject
{
    Q_OBJECT
    Q_PROPERTY(unsigned int courseCount READ courseCount NOTIFY courseCountChanged)
    Q_PROPERTY(unsigned int keyboardLayoutCount READ keyboardLayoutCount NOTIFY keyboardLayoutCountChanged)

public:
    explicit DataIndex(QObject* parent = 0);
    unsigned int courseCount() const;
    Q_INVOKABLE DataIndexCourse* course(unsigned int index) const;
    Q_INVOKABLE void addCourse(DataIndexCourse* course);
    Q_INVOKABLE void removeCourse(unsigned int index);
    Q_INVOKABLE void clearCourses();
    unsigned int keyboardLayoutCount() const;
    Q_INVOKABLE DataIndexKeyboardLayout* keyboardLayout(unsigned int index) const;
    Q_INVOKABLE void addKeyboardLayout(DataIndexKeyboardLayout* keyboardLayout);
    Q_INVOKABLE void removeKeyboardLayout(unsigned int index);
    Q_INVOKABLE void clearKeyboardLayouts();

signals:
    void courseCountChanged();
    void keyboardLayoutCountChanged();

private:
    QList<DataIndexCourse*> m_courses;
    QList<DataIndexKeyboardLayout*> m_keyboardLayouts;
};

#endif // DATAINDEX_H
