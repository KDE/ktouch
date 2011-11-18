#ifndef LESSON_H
#define LESSON_H

#include <QObject>
#include <QString>
#include <QList>

class LessonLine;

class Lesson : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString characters READ characters WRITE setCharacters NOTIFY charactersChanged)
    Q_PROPERTY(int lineCount READ lineCount NOTIFY lineCountChanged)

public:
    explicit Lesson(QObject *parent = 0);

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

    QString characters() const
    {
        return m_characters;
    }

    void setCharacters(const QString& characters)
    {
        if (characters != m_characters)
        {
            m_characters = characters;
            emit charactersChanged(characters);
        }
    }

    int lineCount() const
    {
        return m_lines.count();
    }

    Q_INVOKABLE LessonLine* line(unsigned int index) const;
    Q_INVOKABLE void addLine(LessonLine* line);
    Q_INVOKABLE void removeLine(unsigned int index);
    Q_INVOKABLE void clearLines();

signals:
    void titleChanged(const QString& newTitle);
    void charactersChanged(const QString& newCharacters);
    void lineCountChanged(int newLineCount);

private:
    QString m_title;
    QString m_characters;
    QList<LessonLine*> m_lines;
};

#endif // LESSON_H
