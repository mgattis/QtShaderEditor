#ifndef IPROJECTOBJECT_H
#define IPROJECTOBJECT_H

#include "CPath.h"

#include <iostream>
#include <QString>
#include <QJsonObject>

class IProjectObject {
protected:
    QString itemName;
    QString itemType;
    QDir projectPath;

    QJsonObject userJson;

    bool bIdentifiersLoaded;
    bool bInitializeSuccess;

public:
    IProjectObject();
    ~IProjectObject();

    bool loadUserJson(QJsonObject userJson);
    virtual bool initiaize() = 0;
    virtual bool run(float elapsedTime) = 0;

    QString getItemName();
    void setItemName(QString itemName);
    QString getType();
    void setType(QString itemType);
    QDir getProjectPath();
    void setProjectPath(QDir projectPath);
};

#endif /* !IPROJECTOBJECT_H */
