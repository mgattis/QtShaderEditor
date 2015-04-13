#include "IProjectObject.h"

IProjectObject::IProjectObject() {
    itemName = "";
    itemType = "";
    workingPath = ".";
    bJsonLoaded = false;
    bInitialized = false;

    lpProjectList = NULL;
}

IProjectObject::IProjectObject(QString userJsonFile) {
    itemName = "";
    itemType = "";
    workingPath = ".";
    bJsonLoaded = false;
    bInitialized = false;

    lpProjectList = NULL;

    loadUserJson(userJsonFile);
}

IProjectObject::IProjectObject(QString userJsonFile, QJsonObject userJsonObject) {
    itemName = "";
    itemType = "";
    workingPath = ".";
    bJsonLoaded = false;
    bInitialized = false;

    lpProjectList = NULL;

    loadUserJson(userJsonFile, userJsonObject);
}

IProjectObject::~IProjectObject() {

}

bool IProjectObject::loadUserJson(QString userJsonFile) {
    QFile file(userJsonFile);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    if (file.isOpen()) {
        QString rawData = file.readAll();
        file.close();

        QJsonParseError jError;
        QJsonDocument document = QJsonDocument::fromJson(rawData.toUtf8(), &jError);

        if (jError.error == QJsonParseError::NoError) {
            QJsonObject userJsonObject = document.object();

            bool bSuccess = loadUserJson(userJsonFile, userJsonObject);
            if (bSuccess) {
                return true;
            }
        }
        else {
            std::clog << "[WARNING]: JSON parse error. " << jError.errorString().toStdString() << ". In file '" << userJsonFile.toStdString() << "'." << std::endl;
        }
    }
    else {
        std::clog << "[WARNING]: Unable to open file '" << userJsonFile.toStdString() <<"' for reading." << std::endl;
    }

    bJsonLoaded = false;
    bInitialized = false;
    return false;
}

bool IProjectObject::loadUserJson(QString userJsonFile, QJsonObject userJsonObject) {
    if (userJsonObject.value("itemType").isString() && userJsonObject.value("itemName").isString()) {
        itemName = userJsonObject.value("itemName").toString();
        itemType = userJsonObject.value("itemType").toString();

        if (userJsonFile.compare("") != 0) {
            QDir dir(userJsonFile);
            CPath path(dir.absolutePath());
            workingPath = path.getPath(true);
        }
        else {
            QDir dir("./");
            workingPath = dir.absolutePath();
        }
        this->userJson = userJsonObject;
        bJsonLoaded = true;
        bInitialized = false;

        // std::clog << "[INFO]: '" << itemType.toStdString() << "/" << itemName.toStdString() << "' created." << std::endl;
        // Everything else drops through.
        return true;
    }

    std::clog << "[WARNING]: Invalid project format. '" << userJsonFile.toStdString() << "'." << std::endl;
    return false;
}

QString IProjectObject::getItemName() {
    return itemName;
}

void IProjectObject::setItemName(QString itemName) {
    this->itemName = itemName;
}

QString IProjectObject::getItemType() {
    return itemType;
}

void IProjectObject::setItemType(QString itemType) {
    this->itemType = itemType;
}

QDir IProjectObject::getWorkingPath() {
    return this->workingPath;
}

void IProjectObject::setWorkingPath(QString workingPath) {
    this->workingPath = workingPath;
}

bool IProjectObject::isInitialized() {
    return bInitialized;
}

bool IProjectObject::isJsonLoaded() {
    return bJsonLoaded;
}

void IProjectObject::setViewPort(int iWidth, int iHeight) {
    viewPortWidth = iWidth;
    viewPortHeight = iHeight;
}

void IProjectObject::setCamera(CCamera *camera) {
    this->camera = camera;
}

void IProjectObject::setProjectList(QMap<QString, IProjectObject *> *lpProjectList) {
    this->lpProjectList = lpProjectList;
}

QMap<QString, IProjectObject *> *IProjectObject::getProjectList() {
    return this->lpProjectList;
}
