#ifndef DATAACCESS_H
#define DATAACCESS_H

#include <QObject>

class QXmlSchema;
class QDomDocument;
class QFile;
class DataIndex;
class KeyboardLayout;
class Course;

class DataAccess : public QObject
{
    Q_OBJECT
public:
    explicit DataAccess(QObject *parent = 0);
    Q_INVOKABLE DataIndex* loadDataIndex();
    Q_INVOKABLE KeyboardLayout* loadResourceKeyboardLayout(const QString& relPath);
    Q_INVOKABLE Course* loadResourceCourse(const QString& relPath);

private:
    QXmlSchema loadXmlSchema(const QString& name);
    QDomDocument getDomDocument(QFile& file, QXmlSchema& schema);
    bool openResourceFile(const QString& relPath, QFile& file);

};

#endif // DATAACCESS_H
