#include "dataaccess.h"

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QXmlSchema>
#include <QXmlSchemaValidator>

#include <kstandarddirs.h>
#include <kdebug.h>

#include "dataindex.h"
#include "keyboardlayout.h"

DataAccess::DataAccess(QObject *parent) :
    QObject(parent)
{
}

DataIndex* DataAccess::loadDataIndex()
{
    QFile dataIndexFile;
    if (!openResourceFile("data.xml", dataIndexFile))
        return 0;
    QXmlSchema schema = loadXmlSchema("data");
    if (!schema.isValid())
        return 0;
    QDomDocument doc = getDomDocument(dataIndexFile, schema);
    if (doc.isNull())
    {
        kWarning() << "invalid doc";
        return 0;
    }
    QDomElement root(doc.documentElement());
    DataIndex* dataIndex = new DataIndex(this);
    for (QDomElement dataNode = root.firstChildElement();
         !dataNode.isNull();
         dataNode = dataNode.nextSiblingElement())
    {
        if (dataNode.tagName() == "course")
        {
            DataIndexCourse* course = new DataIndexCourse(this);
            course->setTitle(dataNode.firstChildElement("title").text());
            course->setDescription(dataNode.firstChildElement("description").text());
            course->setKeyboardLayoutName(dataNode.firstChildElement("keyboardLayout").text());
            course->setPath(dataNode.firstChildElement("path").text());
            dataIndex->addCourse(course);
        }
        else if (dataNode.tagName() == "keyboardLayout")
        {
            DataIndexKeyboardLayout* keyboardLayout = new DataIndexKeyboardLayout(this);
            keyboardLayout->setTitle(dataNode.firstChildElement("title").text());
            keyboardLayout->setName(dataNode.firstChildElement("name").text());
            keyboardLayout->setPath(dataNode.firstChildElement("path").text());
            dataIndex->addKeyboardLayout(keyboardLayout);
        }
    }

    return dataIndex;
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
