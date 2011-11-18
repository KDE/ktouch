#ifndef COURSE_H
#define COURSE_H

#include <QObject>
#include <QString>
#include <QList>

class QXmlSchema;
class QDomDocument;
class QIODevice;
class Lesson;

class Course : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString keyboardLayoutName READ keyboardLayoutName WRITE setKeyboardLayoutName NOTIFY keyboardLayoutNameChanged)
    Q_PROPERTY(int lessonCount READ lessonCount NOTIFY lessonCountChanged)

public:
    explicit Course(QObject *parent = 0);
    bool loadXML(QIODevice* dev);

    QString title() const
    {
        return m_title;
    }

    void setTitle(const QString& title)
    {
        if (title != m_title)
        {
            m_title = title;
            emit titleChanged(title);
        }
    }

    QString description() const
    {
        return m_description;
    }

    void setDescription(const QString& description)
    {
        if (description != m_description)
        {
            m_description = description;
            emit descriptionChanged(description);
        }
    }

    QString keyboardLayoutName() const
    {
        return m_keyboardLayoutName;
    }

    void setKeyboardLayoutName(const QString& keyboardLayoutName)
    {
        if (keyboardLayoutName != m_keyboardLayoutName)
        {
            m_keyboardLayoutName = keyboardLayoutName;
            emit keyboardLayoutNameChanged(keyboardLayoutName);
        }
    }

    int lessonCount() const
    {
        return m_lessons.count();
    }

    Q_INVOKABLE Lesson* lesson(unsigned int index) const;
    Q_INVOKABLE void addLesson(Lesson* lesson);
    Q_INVOKABLE void removeLesson(unsigned int index);
    Q_INVOKABLE void clearLessons();



signals:
    void titleChanged(const QString& newTitle);
    void descriptionChanged(const QString& newDescription);
    void keyboardLayoutNameChanged(const QString& newKeyboardLayoutName);
    void lessonCountChanged(int newLessonCount);

private:
    static QXmlSchema* xmlSchema();
    bool parseXML(QDomDocument* doc);
    static QXmlSchema* s_schema;
    QString m_title;
    QString m_description;
    QString m_keyboardLayoutName;
    QList<Lesson*> m_lessons;
};

#endif // COURSE_H
