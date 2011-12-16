#ifndef DATAACCESS_H
#define DATAACCESS_H

#include <QObject>

#include <KUrl>

class QXmlSchema;
class QDomDocument;
class QFile;
class DataIndex;
class KeyboardLayout;

class DataAccess : public QObject
{
    Q_OBJECT
public:
    explicit DataAccess(QObject *parent = 0);
    Q_INVOKABLE DataIndex* loadDataIndex();
    Q_INVOKABLE KeyboardLayout* loadKeyboardLayout(KUrl url);

private:
    QXmlSchema loadXmlSchema(const QString& name);
    QDomDocument getDomDocument(QFile& file, QXmlSchema& schema);
    bool openResourceFile(const QString& relPath, QFile& file);

};

#endif // DATAACCESS_H
