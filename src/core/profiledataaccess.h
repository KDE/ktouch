#ifndef PROFILEDATAACCESS_H
#define PROFILEDATAACCESS_H

#include <QObject>
#include <QList>

class QSqlDatabase;
class QSqlError;
class Profile;

class ProfileDataAccess : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int profileCount READ profileCount NOTIFY profileCountChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
public:
    explicit ProfileDataAccess(QObject* parent = 0);
    Q_INVOKABLE void loadProfiles();
    int profileCount() const;
    Q_INVOKABLE Profile* profile(unsigned int index);
    Q_INVOKABLE Profile* createProfile();
    Q_INVOKABLE void addProfile(Profile* profile);
    Q_INVOKABLE void updateProfile(unsigned int index);
    Q_INVOKABLE void removeProfile(unsigned int index);
    QString errorMessage() const;
signals:
    void profileCountChanged();
    void errorMessageChanged();
private:
    QSqlDatabase database();
    bool checkDbSchema();
    void raiseError(const QSqlError& error);
    QString m_errorMessage;
    QList<Profile*> m_profiles;
};

#endif // PROFILEDATAACCESS_H
