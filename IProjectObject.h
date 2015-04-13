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

    int viewPortWidth;
    int viewPortHeight;
    CCamera *camera;

    bool bJsonLoaded;
    bool bInitialized;

public:
    IProjectObject();
    IProjectObject(QString userJsonFile);
    IProjectObject(QString userJsonFile, QJsonObject userJsonObject);
    ~IProjectObject();

    bool loadUserJson(QString userJsonFile);
    bool loadUserJson(QString userJsonFile, QJsonObject userJsonObject);

    bool isJsonLoaded();
    bool isInitialized();

    QString getItemName();
    void setItemName(QString itemName);

    QString getItemType();
    void setItemType(QString itemType);

    QDir getWorkingPath();
    void setWorkingPath(QString workingPath);

    QMap<QString, IProjectObject *> *getProjectList();

public:
    virtual bool initialize() = 0;

    virtual void setViewPort(int iWidth, int iHeight);
    virtual void setCamera(CCamera *camera);
    virtual void setProjectList(QMap<QString, IProjectObject *> *lpProjectList);
};

#endif /* !IPROJECTOBJECT_H */
