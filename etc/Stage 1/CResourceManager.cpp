#include "CResourceManager.h"

#include <iostream>

CResourceManager::CResourceManager() {

}

CResourceManager::~CResourceManager() {

}

bool CResourceManager::addProjectObject(IProjectObject *object) {
    QString itemName = object->getItemName();

    if (!projectList.contains(itemName)) {
        this->projectList[itemName] = object;
        return true;
    }

    return false;
}

bool CResourceManager::removeProjectObject(QString itemName) {
    int iResult = projectList.remove(itemName);

    if (iResult) {
        return true;
    }

    return false;
}

bool CResourceManager::existInProject(QString itemName) {
    if (projectList.contains(itemName)) {
        return true;
    }

    return false;
}

IProjectObject *CResourceManager::getProjectObject(QString itemName) {
    if (projectList.contains(itemName)) {
        return projectList[itemName];
    }

    return NULL;
}

QMap<QString, IProjectObject *> CResourceManager::getAllProjectObjects() {
    return this->projectList;
}

void CResourceManager::clearLists() {
    projectList.clear();
}

CResourceManager *CResourceManager::instance() {
    static CResourceManager *inst = NULL;
    if (!inst) {
        inst = new CResourceManager();
    }
    return inst;
}
