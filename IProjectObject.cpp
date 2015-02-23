#include "IProjectObject.h"

IProjectObject::IProjectObject() {
    itemName = "";
    itemType = "";
    bIdentifiersLoaded = false;
    bInitializeSuccess = false;
}

IProjectObject::~IProjectObject() {

}

bool IProjectObject::loadUserJson(QJsonObject userJson) {
    if (userJson[QString("itemName")].isString()) {
        if (userJson["itemType"].isString()) {
            itemName = userJson["itemName"].toString();
            itemType = userJson["itemType"].toString();

            this->userJson = userJson;
            bIdentifiersLoaded = true;
            bInitializeSuccess = false;

            return true;
        }
    }

    bIdentifiersLoaded = false;
    bInitializeSuccess = false;
    return false;
}

QString IProjectObject::getItemName() {
    return itemName;
}

void IProjectObject::setItemName(QString itemName) {
    this->itemName = itemName;
}

QString IProjectObject::getType() {
    return itemType;
}

void IProjectObject::setType(QString itemType) {
    this->itemType = itemType;
}

QDir IProjectObject::getProjectPath() {
    return this->projectPath;
}

void IProjectObject::setProjectPath(QDir projectPath) {
    this->projectPath = projectPath;
}
