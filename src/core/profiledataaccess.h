#ifndef PROFILEDATAACCESS_H
#define PROFILEDATAACCESS_H

#include <QObject>

class ProfileDataAccess : public QObject
{
    Q_OBJECT
public:
    explicit ProfileDataAccess(QObject* parent = 0);
private:
    void initDb();
    void checkDbSchema();
};

#endif // PROFILEDATAACCESS_H
