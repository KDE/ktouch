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

#include "dataaccess.h"

#include <QFile>
#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QXmlSchema>
#include <QXmlSchemaValidator>

#include <kstandarddirs.h>
#include <klocale.h>
#include <kdebug.h>

#include "dataindex.h"
#include "keyboardlayout.h"
#include "key.h"
#include "specialkey.h"
#include "keychar.h"
#include "course.h"
#include "lesson.h"
#include "lessonline.h"

DataAccess::DataAccess(QObject *parent) :
    QObject(parent)
{
}

bool DataAccess::loadDataIndex(DataIndex* target)
{
    target->setIsValid(false);
    target->clearCourses();
    target->clearKeyboardLayouts();

    QXmlSchema schema = loadXmlSchema("data");
    if (!schema.isValid())
        return false;

    const QDir userDir = QDir(KGlobal::dirs()->saveLocation("appdata", "", false));

    foreach (const QString path, KGlobal::dirs()->findAllResources("appdata", "data.xml"))
    {
        QDir dir = QFileInfo(path).dir();
        DataIndex::Source source = dir == userDir? DataIndex::UserResource: DataIndex::BuiltInResource;
        QFile dataIndexFile;
        dataIndexFile.setFileName(path);
        if (!dataIndexFile.open(QIODevice::ReadOnly))
        {
            kWarning() << "can't open:" << path;
            return false;
        }
        QDomDocument doc = getDomDocument(dataIndexFile, schema);
        if (doc.isNull())
        {
            kWarning() << "invalid doc:" << path;
            return false;
        }
        QDomElement root(doc.documentElement());
        for (QDomElement dataNode = root.firstChildElement();
            !dataNode.isNull();
            dataNode = dataNode.nextSiblingElement())
        {
            const QString path = dir.filePath(dataNode.firstChildElement("path").text());

            if (dataNode.tagName() == "course")
            {
                DataIndexCourse* course = new DataIndexCourse(this);
                course->setTitle(dataNode.firstChildElement("title").text());
                course->setDescription(dataNode.firstChildElement("description").text());
                course->setKeyboardLayoutName(dataNode.firstChildElement("keyboardLayout").text());
                course->setPath(path);
                course->setSource(source);
                target->addCourse(course);
            }
            else if (dataNode.tagName() == "keyboardLayout")
            {
                DataIndexKeyboardLayout* keyboardLayout = new DataIndexKeyboardLayout(this);
                keyboardLayout->setTitle(dataNode.firstChildElement("title").text());
                keyboardLayout->setName(dataNode.firstChildElement("name").text());
                keyboardLayout->setPath(path);
                keyboardLayout->setSource(source);
                target->addKeyboardLayout(keyboardLayout);
            }
        }
    }

    target->setIsValid(true);
    return true;
}

bool DataAccess::storeDataIndex(DataIndex* source)
{
    const QDir userDir = QDir(KGlobal::dirs()->saveLocation("appdata", "", true));

    QDomDocument doc;

    QDomProcessingInstruction header = doc.createProcessingInstruction("xml", "version=\"1.0\"");
    doc.appendChild(header);

    QDomElement root = doc.createElement("data");
    doc.appendChild(root);

    for (int i = 0; i < source->courseCount(); i++)
    {
        DataIndexCourse* const course = source->course(i);

        if (course->source() == DataIndex::UserResource)
        {
            const QString relPath = userDir.relativeFilePath(course->path());

            QDomElement courseElem = doc.createElement("course");
            QDomElement titleElem = doc.createElement("title");
            QDomElement descriptionElem = doc.createElement("description");
            QDomElement keyboardLayoutElem = doc.createElement("keyboardLayout");
            QDomElement pathElem = doc.createElement("path");

            titleElem.appendChild(doc.createTextNode(course->title()));
            descriptionElem.appendChild(doc.createTextNode(course->description()));
            keyboardLayoutElem.appendChild(doc.createTextNode(course->keyboardLayoutName()));
            pathElem.appendChild(doc.createTextNode(relPath));

            courseElem.appendChild(titleElem);
            courseElem.appendChild(descriptionElem);
            courseElem.appendChild(keyboardLayoutElem);
            courseElem.appendChild(pathElem);
            root.appendChild(courseElem);
        }
    }

    for (int i = 0; i < source->keyboardLayoutCount(); i++)
    {
        DataIndexKeyboardLayout* const keyboardLayout = source->keyboardLayout(i);

        if (keyboardLayout->source() == DataIndex::UserResource)
        {
            const QString relPath = userDir.relativeFilePath(keyboardLayout->path());

            QDomElement keyboardLayoutElem = doc.createElement("keyboardLayout");
            QDomElement titleElem = doc.createElement("title");
            QDomElement nameElem = doc.createElement("name");
            QDomElement pathElem = doc.createElement("path");

            titleElem.appendChild(doc.createTextNode(keyboardLayout->title()));
            nameElem.appendChild(doc.createTextNode(keyboardLayout->name()));
            pathElem.appendChild(doc.createTextNode(relPath));

            keyboardLayoutElem.appendChild(titleElem);
            keyboardLayoutElem.appendChild(nameElem);
            keyboardLayoutElem.appendChild(pathElem);
            root.appendChild(keyboardLayoutElem);
        }
    }

    QFile file;
    file.setFileName(userDir.filePath("data.xml"));

    if (!file.open(QIODevice::WriteOnly))
    {
        kWarning() << "can't open:" << file.fileName();
        return false;
    }

    file.write(doc.toByteArray());
    return true;
}

bool DataAccess::loadKeyboardLayout(const QString &path, KeyboardLayout* target)
{
    target->setIsValid(false);

    QFile keyboardLayoutFile;
    keyboardLayoutFile.setFileName(path);
    if (!keyboardLayoutFile.open(QIODevice::ReadOnly))
    {
        kWarning() << "can't open:" << path;
        return false;
    }
    QXmlSchema schema = loadXmlSchema("keyboardlayout");
    if (!schema.isValid())
        return false;
    QDomDocument doc = getDomDocument(keyboardLayoutFile, schema);
    if (doc.isNull())
    {
        kWarning() << "invalid doc:" << path;
        return false;
    }
    QDomElement root(doc.documentElement());

    target->clearKeys();
    target->setTitle(i18nc("Keyboard Layout Name", root.firstChildElement("title").text().toUtf8()));
    target->setName(root.firstChildElement("name").text());
    target->setWidth(root.firstChildElement("width").text().toInt());
    target->setHeight(root.firstChildElement("height").text().toInt());
    for (QDomElement keyNode = root.firstChildElement("keys").firstChildElement();
         !keyNode.isNull();
         keyNode = keyNode.nextSiblingElement())
    {
        AbstractKey* abstractKey;

        if (keyNode.tagName() == "key")
        {
            Key* key = new Key(this);
            key->setFingerIndex(keyNode.attribute("fingerIndex").toInt());
            key->setHasHapticMarker(keyNode.attribute("hasHapticMarker") == "true");
            for (QDomElement charNode = keyNode.firstChildElement("char");
                 !charNode.isNull();
                 charNode = charNode.nextSiblingElement("char"))
            {
                KeyChar* keyChar = new KeyChar(key);
                keyChar->setValue(charNode.text().at(0));
                keyChar->setPositionStr(charNode.attribute("position"));
                keyChar->setModifier(charNode.attribute("modifier"));
                key->addKeyChar(keyChar);
            }
            abstractKey = key;
        }
        else if (keyNode.tagName() == "specialKey")
        {
            SpecialKey* specialKey = new SpecialKey(this);
            specialKey->setTypeStr(keyNode.attribute("type"));
            specialKey->setModifierId(keyNode.attribute("modifierId"));
            specialKey->setLabel(keyNode.attribute("label"));
            abstractKey = specialKey;
        }
        else
        {
            continue;
        }
        abstractKey->setLeft(keyNode.attribute("left").toInt());
        abstractKey->setTop(keyNode.attribute("top").toInt());
        abstractKey->setWidth(keyNode.attribute("width").toInt());
        abstractKey->setHeight(keyNode.attribute("height").toInt());
        target->addKey(abstractKey);
    }
    kDebug() << "read" << target->title() << "with" << target->keyCount() << "keys";

    target->setIsValid(true);
    return true;
}

bool DataAccess::storeKeyboardLayout(const QString& path, KeyboardLayout* source)
{
    QDomDocument doc;

    QDomProcessingInstruction header = doc.createProcessingInstruction("xml", "version=\"1.0\"");
    doc.appendChild(header);

    QDomElement root = doc.createElement("keyboardLayout");
    doc.appendChild(root);

    QDomElement titleElem = doc.createElement("title");
    QDomElement nameElem = doc.createElement("name");
    QDomElement widthElem = doc.createElement("width");
    QDomElement heightElem = doc.createElement("height");
    QDomElement keysElem = doc.createElement("keys");

    titleElem.appendChild(doc.createTextNode(source->title()));
    nameElem.appendChild(doc.createTextNode(source->name()));
    heightElem.appendChild(doc.createTextNode(QString::number(source->height())));
    widthElem.appendChild(doc.createTextNode(QString::number(source->width())));

    for (int i = 0; i < source->keyCount(); i++)
    {
        AbstractKey* const abstractKey = source->key(i);

        QDomElement keyElem = doc.createElement("key");

        keyElem.setAttribute("left", abstractKey->left());
        keyElem.setAttribute("top", abstractKey->top());
        keyElem.setAttribute("width", abstractKey->width());
        keyElem.setAttribute("height", abstractKey->height());

        if (Key* const key = qobject_cast<Key*>(abstractKey))
        {
            keyElem.setTagName("key");
            keyElem.setAttribute("fingerIndex", key->fingerIndex());
            if (key->hasHapticMarker())
            {
                keyElem.setAttribute("hasHapticMarker", "true");
            }

            for (int j = 0; j < key->keyCharCount(); j++)
            {
                KeyChar* const keyChar = key->keyChar(j);

                QDomElement keyCharElem = doc.createElement("char");
                keyCharElem.setAttribute("position", keyChar->positionStr());
                const QString modifier = keyChar->modifier();
                if (!modifier.isNull())
                {
                    keyCharElem.setAttribute("modifier", modifier);
                }
                keyCharElem.appendChild(doc.createTextNode(keyChar->value()));
                keyElem.appendChild(keyCharElem);
            }
        }

        if (SpecialKey* const specialKey = qobject_cast<SpecialKey*>(abstractKey))
        {
            keyElem.setTagName("specialKey");
            keyElem.setAttribute("type", specialKey->typeStr());

            const QString modifierId = specialKey->modifierId();
            if (!modifierId.isNull())
            {
                keyElem.setAttribute("modifierId", modifierId);
            }
            const QString label = specialKey->modifierId();
            if (!label.isNull())
            {
                keyElem.setAttribute("label", label);
            }
        }

        keysElem.appendChild(keyElem);
    }

    root.appendChild(titleElem);
    root.appendChild(nameElem);
    root.appendChild(widthElem);
    root.appendChild(heightElem);
    root.appendChild(keysElem);

    QFile file;

    file.setFileName(path);

    if (!file.open(QIODevice::WriteOnly))
    {
        kWarning() << "can't open:" << file.fileName();
        return false;
    }

    file.write(doc.toByteArray());
    return true;
}


bool DataAccess::loadCourse(const QString &path, Course* target)
{
    target->setIsValid(false);
    QFile courseFile;
    courseFile.setFileName(path);
    if (!courseFile.open(QIODevice::ReadOnly))
    {
        kWarning() << "can't open:" << path;
        return false;
    }
    QXmlSchema schema = loadXmlSchema("course");
    if (!schema.isValid())
        return false;
    QDomDocument doc = getDomDocument(courseFile, schema);
    if (doc.isNull())
    {
        kWarning() << "invalid doc:" << path;
        return false;
    }
    QDomElement root(doc.documentElement());

    target->setId(root.firstChildElement("id").text());
    target->setTitle(i18nc("course title", root.firstChildElement("title").text().toUtf8()));
    target->setDescription(i18nc("course description", root.firstChildElement("description").text().toUtf8()));
    target->setKeyboardLayoutName(root.firstChildElement("keyboardLayout").text());
    target->clearLessons();

    for (QDomElement lessonNode = root.firstChildElement("lessons").firstChildElement();
         !lessonNode.isNull();
         lessonNode = lessonNode.nextSiblingElement())
    {
        Lesson* lesson = new Lesson(this);
        lesson->setId(lessonNode.firstChildElement("id").text());
        lesson->setTitle(i18nc("lesson title", lessonNode.firstChildElement("title").text().toUtf8()));
        lesson->setNewCharacters(lessonNode.firstChildElement("newCharacters").text());
        const QString text = lessonNode.firstChildElement("text").text();
        const QStringList lines = text.split("\n");
        foreach(const QString lineStr, lines)
        {
            LessonLine* line = new LessonLine(lesson);
            line->setValue(lineStr);
            lesson->addLine(line);
        }
        target->addLesson(lesson);
    }

    target->setIsValid(true);
    return true;
}

bool DataAccess::storeCourse(const QString& path, Course* source)
{

    QDomDocument doc;

    QDomProcessingInstruction header = doc.createProcessingInstruction("xml", "version=\"1.0\"");
    doc.appendChild(header);

    QDomElement root = doc.createElement("course");
    doc.appendChild(root);

    QDomElement idElem = doc.createElement("id");
    QDomElement titleElem = doc.createElement("title");
    QDomElement descriptionElem = doc.createElement("description");
    QDomElement keyboardLayoutElem = doc.createElement("keyboardLayout");
    QDomElement lessonsElem = doc.createElement("lessons");

    idElem.appendChild(doc.createTextNode(source->id()));
    titleElem.appendChild(doc.createTextNode(source->title()));
    keyboardLayoutElem.appendChild(doc.createTextNode(source->keyboardLayoutName()));
    descriptionElem.appendChild(doc.createTextNode(source->description()));

    for (int i = 0; i < source->lessonCount(); i++)
    {
        Lesson* const lesson = source->lesson(i);

        QStringList lines;

        for (int j = 0; j < lesson->lineCount(); j++)
        {
            lines << lesson->line(j)->value();
        }

        QDomElement lessonElem = doc.createElement("lesson");
        QDomElement idElem = doc.createElement("id");
        QDomElement titleElem = doc.createElement("title");
        QDomElement newCharactersElem = doc.createElement("newCharacters");
        QDomElement textElem = doc.createElement("text");

        idElem.appendChild(doc.createTextNode(lesson->id()));
        titleElem.appendChild(doc.createTextNode(lesson->title()));
        newCharactersElem.appendChild(doc.createTextNode(lesson->newCharacters()));
        textElem.appendChild(doc.createTextNode(lines.join("\n")));

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
        kWarning() << "can't open:" << file.fileName();
        return false;
    }

    file.write(doc.toByteArray());
    return true;
}

QXmlSchema DataAccess::loadXmlSchema(const QString &name)
{
    QXmlSchema schema;
    QString relPath = QString("schemata/%1.xsd").arg(name);
    QFile schemaFile;
    if (!openResourceFile(relPath, schemaFile))
    {
        return schema;
    }
    schema.load(&schemaFile, QUrl::fromLocalFile(schemaFile.fileName()));
    if (!schema.isValid())
    {
        kWarning() << schemaFile.fileName() << "is invalid";
    }
    return schema;
}

QDomDocument DataAccess::getDomDocument(QFile &file, QXmlSchema &schema)
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
        kWarning() << errorMsg;
    }
    return doc;
}

bool DataAccess::openResourceFile(const QString &relPath, QFile& file)
{
    QString path = KGlobal::dirs()->findResource("appdata", relPath);
    if (path.isNull())
    {
        kWarning() << "can't find resource:" << relPath;
        return false;
    }
    file.setFileName(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        kWarning() << "can't open" << path;
        return false;
    }
    return true;
}
