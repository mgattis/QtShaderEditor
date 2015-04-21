#ifndef IPROJECTOBJECT_H
#define IPROJECTOBJECT_H

#include "CPath.h"
#include "CCamera.h"

#include <iostream>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

class IProjectObject {
protected:
    QMap<QString, IProjectObject *> *lpProjectList;

    QString itemName;
    QString itemType;
    QString workingPath;

    QJsonObject userJson;

    int iViewPortWidth;
    int iViewPortHeight;
    CCamera *camera;

    bool bJsonLoaded;
    bool bInitialized;

public:
    IProjectObject();
    IProjectObject(QString userJsonFile);
    IProjectObject(QString userJsonFile, QJsonObject userJsonObject);
    virtual ~IProjectObject();

    bool loadUserJson(QString userJsonFile);
    bool loadUserJson(QString userJsonFile, QJsonObject userJsonObject);

    bool isJsonLoaded();
    bool isInitialized();

    QString getItemName();
    void setItemName(QString itemName);

    QString getItemType();
    void setItemType(QString itemType);

    QString getWorkingPath();
    void setWorkingPath(QString workingPath);

    QMap<QString, IProjectObject *> *getProjectList();

    QString getFullIdentifier();

public:
    // Logging stuff to make logging easy!
    void logInfo(QString message);
    void logWarning(QString message);
    void logError(QString message);

public:
    virtual bool initialize() = 0;

    virtual void setViewPort(int iWidth, int iHeight);
    virtual void setCamera(CCamera *camera);
    virtual void setRunTime(float fRunTime);
    virtual void setProjectList(QMap<QString, IProjectObject *> *lpProjectList);
};

#endif /* !IPROJECTOBJECT_H */
