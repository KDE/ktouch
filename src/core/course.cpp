#include "course.h"

#include <QFile>
#include <QUrl>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QXmlSchema>
#include <QXmlSchemaValidator>

#include <kstandarddirs.h>
#include <klocale.h>
#include <kdebug.h>

#include "lesson.h"
#include "lessonline.h"

Course::Course(QObject *parent) :
    CourseBase(parent)
{
}

bool Course::loadXML(QIODevice *dev)
{
    QXmlSchema* schema = xmlSchema();
    if (!schema)
    {
        return false;
    }
    QXmlSchemaValidator validator;
    validator.setSchema(*schema);
    if (!validator.validate(dev))
    {
        kWarning() << "invalid course XML";
        return false;
    }
    dev->reset();
    QDomDocument doc;
    QString errorMsg;
    if (!doc.setContent(dev, &errorMsg))
    {
        kWarning() << errorMsg;
        return false;
    }
    return parseXML(&doc);
}

Lesson* Course::lesson(unsigned int index) const
{
    Q_ASSERT(index < m_lessons.count());
    return m_lessons.at(index);
}

void Course::addLesson(Lesson* lesson)
{
    m_lessons.append(lesson);
    lesson->setParent(this);
    emit lessonCountChanged(m_lessons.count());
}

void Course::removeLesson(unsigned int index)
{
    Q_ASSERT(index < m_lessons.count());
    delete m_lessons.at(index);
    m_lessons.removeAt(index);
    emit lessonCountChanged(m_lessons.count());
}

void Course::clearLessons()
{
    qDeleteAll(m_lessons);
    m_lessons.clear();
    emit lessonCountChanged(m_lessons.count());
}

QXmlSchema* Course::xmlSchema()
{
    if (!s_schema)
    {
        QString resourceName = "schemata/course.xsd";
        QString schemaPath = KGlobal::dirs()->findResource("appdata", resourceName);
        if (schemaPath.isNull())
        {
            kWarning() << "can't find resource" << resourceName;
            return 0;
        }
        QFile schemaFile(schemaPath);
        if (!schemaFile.open(QIODevice::ReadOnly))
        {
            kWarning() << "can't open" << schemaPath;
            return 0;
        }
        QXmlSchema* schema = new QXmlSchema();
        schema->load(&schemaFile, QUrl::fromLocalFile(schemaPath));
        if (!schema->isValid())
        {
            kWarning() << resourceName << "is invalid";
            delete schema;
            return 0;
        }
        s_schema = schema;
    }
    return s_schema;
}

bool Course::parseXML(QDomDocument *doc)
{
    QDomElement root = doc->documentElement();
    setTitle(i18nc("course title", root.firstChildElement("title").text().toUtf8()));
    setDescription(i18nc("course description", root.firstChildElement("description").text().toUtf8()));
    setKeyboardLayoutName(root.firstChildElement("keyboardLayout").text());
    clearLessons();
    QString allowedChars = "";
    for (QDomElement lessonNode = root.firstChildElement("lessons").firstChildElement();
         !lessonNode.isNull();
         lessonNode = lessonNode.nextSiblingElement())
    {
        Lesson* lesson = new Lesson(this);
        lesson->setTitle(i18nc("lesson title", lessonNode.firstChildElement("title").text().toUtf8()));
        QString newChars = lessonNode.firstChildElement("newCharacters").text();
        for (int i = 0; i < newChars.length(); i++)
        {
            QChar newChar = newChars.at(i);
            if (!allowedChars.contains(newChar))
            {
                allowedChars.append(newChar);
            }
        }
        lesson->setCharacters(allowedChars);
        QString text = lessonNode.firstChildElement("text").text();
        QStringList lines = text.split("\n");
        foreach(QString lineStr, lines)
        {
            LessonLine* line = new LessonLine(lesson);
            line->setValue(lineStr);
            lesson->addLine(line);
        }
        addLesson(lesson);
    }
    return true;
}

QXmlSchema* Course::s_schema;
