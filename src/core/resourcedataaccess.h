/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef RESOURCEDATAACCESS_H
#define RESOURCEDATAACCESS_H

#include <QObject>

class QXmlSchema;
class QDomDocument;
class QFile;
class DataIndex;
class KeyboardLayout;
class Course;

class ResourceDataAccess : public QObject
{
    Q_OBJECT
public:
    explicit ResourceDataAccess(QObject *parent = 0);
    Q_INVOKABLE bool fillDataIndex(DataIndex* target);
    Q_INVOKABLE bool loadKeyboardLayout(const QString& path, KeyboardLayout* target);
    Q_INVOKABLE bool storeKeyboardLayout(const QString& path, KeyboardLayout* source);
    Q_INVOKABLE bool loadCourse(const QString& path, Course* target);
    Q_INVOKABLE bool storeCourse(const QString& path, Course* source);

private:
    QXmlSchema loadXmlSchema(const QString& name);
    QDomDocument getDomDocument(QFile& file, QXmlSchema& schema);
    bool openResourceFile(const QString& relPath, QFile& file);
};

#endif // RESOURCEDATAACCESS_H
