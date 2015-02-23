#ifndef CRESOURCEMANAGER_H
#define CRESOURCEMANAGER_H

#include "IProjectObject.h"

#include <QString>
#include <QMap>

class CResourceManager {
private:
    QMap<QString, IProjectObject *> projectList;

    CResourceManager();
    ~CResourceManager();

public:
    bool addProjectObject(IProjectObject *object);
    bool removeProjectObject(QString itemName);
    bool existInProject(QString itemName);
    IProjectObject *getProjectObject(QString itemName);
    QMap<QString, IProjectObject *> getAllProjectObjects();

    void clearLists();

    static CResourceManager *instance();
};

#endif // CRESOURCEMANAGER_H
