/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "resourcedataaccess.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QUrl>
#include <QStandardPaths>
#include <QXmlSchema>
#include <QXmlSchemaValidator>


#include "dataindex.h"
#include "keyboardlayout.h"
#include "key.h"
#include "specialkey.h"
#include "keychar.h"
#include "course.h"
#include "lesson.h"

ResourceDataAccess::ResourceDataAccess(QObject *parent) :
    QObject(parent)
{
}

bool ResourceDataAccess::fillDataIndex(DataIndex* target)
{
    QXmlSchema schema = loadXmlSchema(QStringLiteral("data"));
    if (!schema.isValid())
        return false;

    foreach (const QString& path, QStandardPaths::locateAll(QStandardPaths::DataLocation, "data.xml"))
    {
        QDir dir = QFileInfo(path).dir();
        QFile dataIndexFile;
        dataIndexFile.setFileName(path);
        if (!dataIndexFile.open(QIODevice::ReadOnly))
        {
            qWarning() << "can't open:" << path;
            return false;
        }
        QDomDocument doc = getDomDocument(dataIndexFile, schema);
        if (doc.isNull())
        {
            qWarning() << "invalid doc:" << path;
            return false;
        }
        QDomElement root(doc.documentElement());
        for (QDomElement dataNode = root.firstChildElement();
            !dataNode.isNull();
            dataNode = dataNode.nextSiblingElement())
        {
            const QString path = dir.filePath(dataNode.firstChildElement(QStringLiteral("path")).text());

            if (dataNode.tagName() == QLatin1String("course"))
            {
                DataIndexCourse* course = new DataIndexCourse(this);
                course->setTitle(dataNode.firstChildElement(QStringLiteral("title")).text());
                course->setDescription(dataNode.firstChildElement(QStringLiteral("description")).text());
                course->setKeyboardLayoutName(dataNode.firstChildElement(QStringLiteral("keyboardLayout")).text());
                course->setId(dataNode.firstChildElement(QStringLiteral("id")).text());
                course->setPath(path);
                course->setSource(DataIndex::BuiltInResource);
                target->addCourse(course);
            }
            else if (dataNode.tagName() == QLatin1String("keyboardLayout"))
            {
                DataIndexKeyboardLayout* keyboardLayout = new DataIndexKeyboardLayout(this);
                keyboardLayout->setTitle(dataNode.firstChildElement(QStringLiteral("title")).text());
                keyboardLayout->setName(dataNode.firstChildElement(QStringLiteral("name")).text());
                keyboardLayout->setId(dataNode.firstChildElement(QStringLiteral("id")).text());
                keyboardLayout->setPath(path);
                keyboardLayout->setSource(DataIndex::BuiltInResource);
                target->addKeyboardLayout(keyboardLayout);
            }
        }
    }

    return true;
}

bool ResourceDataAccess::loadKeyboardLayout(const QString &path, KeyboardLayout* target)
{
    target->setIsValid(false);

    QFile keyboardLayoutFile;
    keyboardLayoutFile.setFileName(path);
    if (!keyboardLayoutFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "can't open:" << path;
        return false;
    }
    QXmlSchema schema = loadXmlSchema(QStringLiteral("keyboardlayout"));
    if (!schema.isValid())
        return false;
    QDomDocument doc = getDomDocument(keyboardLayoutFile, schema);
    if (doc.isNull())
    {
        qWarning() << "invalid doc:" << path;
        return false;
    }
    QDomElement root(doc.documentElement());

    target->clearKeys();
    target->setId(root.firstChildElement(QStringLiteral("id")).text());
    target->setTitle(root.firstChildElement(QStringLiteral("title")).text());
    target->setName(root.firstChildElement(QStringLiteral("name")).text());
    target->setWidth(root.firstChildElement(QStringLiteral("width")).text().toInt());
    target->setHeight(root.firstChildElement(QStringLiteral("height")).text().toInt());
    for (QDomElement keyNode = root.firstChildElement(QStringLiteral("keys")).firstChildElement();
         !keyNode.isNull();
         keyNode = keyNode.nextSiblingElement())
    {
        AbstractKey* abstractKey;

        if (keyNode.tagName() == QLatin1String("key"))
        {
            Key* key = new Key(this);
            key->setFingerIndex(keyNode.attribute(QStringLiteral("fingerIndex")).toInt());
            key->setHasHapticMarker(keyNode.attribute(QStringLiteral("hasHapticMarker")) == QLatin1String("true"));
            for (QDomElement charNode = keyNode.firstChildElement(QStringLiteral("char"));
                 !charNode.isNull();
                 charNode = charNode.nextSiblingElement(QStringLiteral("char")))
            {
                KeyChar* keyChar = new KeyChar(key);
                keyChar->setValue(charNode.text().at(0));
                keyChar->setPositionStr(charNode.attribute(QStringLiteral("position")));
                keyChar->setModifier(charNode.attribute(QStringLiteral("modifier")));
                key->addKeyChar(keyChar);
            }
            abstractKey = key;
        }
        else if (keyNode.tagName() == QLatin1String("specialKey"))
        {
            SpecialKey* specialKey = new SpecialKey(this);
            specialKey->setTypeStr(keyNode.attribute(QStringLiteral("type")));
            specialKey->setModifierId(keyNode.attribute(QStringLiteral("modifierId")));
            specialKey->setLabel(keyNode.attribute(QStringLiteral("label")));
            abstractKey = specialKey;
        }
        else
        {
            continue;
        }
        abstractKey->setLeft(keyNode.attribute(QStringLiteral("left")).toInt());
        abstractKey->setTop(keyNode.attribute(QStringLiteral("top")).toInt());
        abstractKey->setWidth(keyNode.attribute(QStringLiteral("width")).toInt());
        abstractKey->setHeight(keyNode.attribute(QStringLiteral("height")).toInt());
        target->addKey(abstractKey);
    }

    target->setIsValid(true);
    return true;
}

bool ResourceDataAccess::storeKeyboardLayout(const QString& path, KeyboardLayout* source)
{
    QDomDocument doc;

    QDomProcessingInstruction header = doc.createProcessingInstruction(QStringLiteral("xml"), QStringLiteral("version=\"1.0\""));
    doc.appendChild(header);

    QDomElement root = doc.createElement(QStringLiteral("keyboardLayout"));
    doc.appendChild(root);

    QDomElement idElem = doc.createElement(QStringLiteral("id"));
    QDomElement titleElem = doc.createElement(QStringLiteral("title"));
    QDomElement nameElem = doc.createElement(QStringLiteral("name"));
    QDomElement widthElem = doc.createElement(QStringLiteral("width"));
    QDomElement heightElem = doc.createElement(QStringLiteral("height"));
    QDomElement keysElem = doc.createElement(QStringLiteral("keys"));

    idElem.appendChild(doc.createTextNode(source->id()));
    titleElem.appendChild(doc.createTextNode(source->title()));
    nameElem.appendChild(doc.createTextNode(source->name()));
    heightElem.appendChild(doc.createTextNode(QString::number(source->height())));
    widthElem.appendChild(doc.createTextNode(QString::number(source->width())));

    for (int i = 0; i < source->keyCount(); i++)
    {
        AbstractKey* const abstractKey = source->key(i);

        QDomElement keyElem = doc.createElement(QStringLiteral("key"));

        keyElem.setAttribute(QStringLiteral("left"), abstractKey->left());
        keyElem.setAttribute(QStringLiteral("top"), abstractKey->top());
        keyElem.setAttribute(QStringLiteral("width"), abstractKey->width());
        keyElem.setAttribute(QStringLiteral("height"), abstractKey->height());

        if (Key* const key = qobject_cast<Key*>(abstractKey))
        {
            keyElem.setTagName(QStringLiteral("key"));
            keyElem.setAttribute(QStringLiteral("fingerIndex"), key->fingerIndex());
            if (key->hasHapticMarker())
            {
                keyElem.setAttribute(QStringLiteral("hasHapticMarker"), QStringLiteral("true"));
            }

            for (int j = 0; j < key->keyCharCount(); j++)
            {
                KeyChar* const keyChar = key->keyChar(j);

                QDomElement keyCharElem = doc.createElement(QStringLiteral("char"));
                keyCharElem.setAttribute(QStringLiteral("position"), keyChar->positionStr());
                const QString modifier = keyChar->modifier();
                if (!modifier.isEmpty())
                {
                    keyCharElem.setAttribute(QStringLiteral("modifier"), modifier);
                }
                const QString value = keyChar->value();
                if (value == QLatin1Char(' '))
                {
                    keyCharElem.appendChild(doc.createCDATASection(value));
                }
                else
                {
                    keyCharElem.appendChild(doc.createTextNode(value));
                }
                keyElem.appendChild(keyCharElem);
            }
        }

        if (SpecialKey* const specialKey = qobject_cast<SpecialKey*>(abstractKey))
        {
            keyElem.setTagName(QStringLiteral("specialKey"));
            keyElem.setAttribute(QStringLiteral("type"), specialKey->typeStr());

            const QString modifierId = specialKey->modifierId();
            if (!modifierId.isNull())
            {
                keyElem.setAttribute(QStringLiteral("modifierId"), modifierId);
            }
            const QString label = specialKey->label();
            if (!label.isNull())
            {
                keyElem.setAttribute(QStringLiteral("label"), label);
            }
        }

        keysElem.appendChild(keyElem);
    }

    root.appendChild(idElem);
    root.appendChild(titleElem);
    root.appendChild(nameElem);
    root.appendChild(widthElem);
    root.appendChild(heightElem);
    root.appendChild(keysElem);

    QFile file;

    file.setFileName(path);

    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "can't open:" << file.fileName();
        return false;
    }

    file.write(doc.toByteArray());
    return true;
}


bool ResourceDataAccess::loadCourse(const QString &path, Course* target)
{
    target->setIsValid(false);
    QFile courseFile;
    courseFile.setFileName(path);
    if (!courseFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "can't open:" << path;
        return false;
    }
    QXmlSchema schema = loadXmlSchema(QStringLiteral("course"));
    if (!schema.isValid())
        return false;
    QDomDocument doc = getDomDocument(courseFile, schema);
    if (doc.isNull())
    {
        qWarning() << "invalid doc:" << path;
        return false;
    }
    QDomElement root(doc.documentElement());

    target->setId(root.firstChildElement(QStringLiteral("id")).text());
    target->setTitle(root.firstChildElement(QStringLiteral("title")).text());
    target->setDescription(root.firstChildElement(QStringLiteral("description")).text());
    target->setKeyboardLayoutName(root.firstChildElement(QStringLiteral("keyboardLayout")).text());
    target->setKind(Course::SequentialCourse);
    target->clearLessons();

    for (QDomElement lessonNode = root.firstChildElement(QStringLiteral("lessons")).firstChildElement();
         !lessonNode.isNull();
         lessonNode = lessonNode.nextSiblingElement())
    {
        Lesson* lesson = new Lesson(this);
        lesson->setId(lessonNode.firstChildElement(QStringLiteral("id")).text());
        lesson->setTitle(lessonNode.firstChildElement(QStringLiteral("title")).text());
        lesson->setNewCharacters(lessonNode.firstChildElement(QStringLiteral("newCharacters")).text());
        lesson->setText(lessonNode.firstChildElement(QStringLiteral("text")).text());
        target->addLesson(lesson);
    }

    target->setIsValid(true);
    return true;
}

bool ResourceDataAccess::storeCourse(const QString& path, Course* source)
{

    QDomDocument doc;

    QDomProcessingInstruction header = doc.createProcessingInstruction(QStringLiteral("xml"), QStringLiteral("version=\"1.0\""));
    doc.appendChild(header);

    QDomElement root = doc.createElement(QStringLiteral("course"));
    doc.appendChild(root);

    QDomElement idElem = doc.createElement(QStringLiteral("id"));
    QDomElement titleElem = doc.createElement(QStringLiteral("title"));
    QDomElement descriptionElem = doc.createElement(QStringLiteral("description"));
    QDomElement keyboardLayoutElem = doc.createElement(QStringLiteral("keyboardLayout"));
    QDomElement lessonsElem = doc.createElement(QStringLiteral("lessons"));

    idElem.appendChild(doc.createTextNode(source->id()));
    titleElem.appendChild(doc.createTextNode(source->title()));
    keyboardLayoutElem.appendChild(doc.createTextNode(source->keyboardLayoutName()));
    descriptionElem.appendChild(doc.createTextNode(source->description()));

    for (int i = 0; i < source->lessonCount(); i++)
    {
        Lesson* const lesson = source->lesson(i);

        QStringList lines;

        QDomElement lessonElem = doc.createElement(QStringLiteral("lesson"));
        QDomElement idElem = doc.createElement(QStringLiteral("id"));
        QDomElement titleElem = doc.createElement(QStringLiteral("title"));
        QDomElement newCharactersElem = doc.createElement(QStringLiteral("newCharacters"));
        QDomElement textElem = doc.createElement(QStringLiteral("text"));

        idElem.appendChild(doc.createTextNode(lesson->id()));
        titleElem.appendChild(doc.createTextNode(lesson->title()));
        newCharactersElem.appendChild(doc.createTextNode(lesson->newCharacters()));
        textElem.appendChild(doc.createTextNode(lesson->text()));

        lessonElem.appendChild(idElem);
        lessonElem.appendChild(titleElem);
        lessonElem.appendChild(newCharactersElem);
        lessonElem.appendChild(textElem);
        lessonsElem.appendChild(lessonElem);
    }

    root.appendChild(idElem);
    root.appendChild(titleElem);
    root.appendChild(descriptionElem);
    root.appendChild(keyboardLayoutElem);
    root.appendChild(lessonsElem);

    QFile file;

    file.setFileName(path);

    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "can't open:" << file.fileName();
        return false;
    }

    file.write(doc.toByteArray());
    return true;
}

QXmlSchema ResourceDataAccess::loadXmlSchema(const QString &name)
{
    QXmlSchema schema;
    QString relPath = QStringLiteral("schemata/%1.xsd").arg(name);
    QFile schemaFile;
    if (!openResourceFile(relPath, schemaFile))
    {
        return schema;
    }
    schema.load(&schemaFile, QUrl::fromLocalFile(schemaFile.fileName()));
    if (!schema.isValid())
    {
        qWarning() << schemaFile.fileName() << "is invalid";
    }
    return schema;
}

QDomDocument ResourceDataAccess::getDomDocument(QFile &file, QXmlSchema &schema)
{
    QDomDocument doc;
    QXmlSchemaValidator validator(schema);
    if (!validator.validate(&file))
    {
        return doc;
    }
    file.reset();
    QString errorMsg;
    if (!doc.setContent(&file, &errorMsg))
    {
        qWarning() << errorMsg;
    }
    return doc;
}

bool ResourceDataAccess::openResourceFile(const QString &relPath, QFile& file)
{
    QString path = QStandardPaths::locate(QStandardPaths::DataLocation, relPath);
    if (path.isNull())
    {
        qWarning() << "can't find resource:" << relPath;
        return false;
    }
    file.setFileName(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "can't open" << path;
        return false;
    }
    return true;
}
