#include "CProject.h"

#include <QJsonParseError>

CProject::CProject() {
    projectVersion = "v0.1b";
    cameraType = "freeCamera";
    fSpeedMultiplier = 1.0;
    lpProjectList = &projectList;
}

CProject::CProject(QString projectFile) : IProjectObject(projectFile) {
    projectVersion = "v0.1b";
    cameraType = "freeCamera";
    fSpeedMultiplier = 1.0;
    lpProjectList = &projectList;

    openProject(projectFile);
}

CProject::~CProject() {
    closeProject();
}

void CProject::_loadValuesFromUserJson() {
    if (isJsonLoaded() == true) {
        if (userJson.value("projectVersion").isString()) {
            projectVersion = userJson.value("projectVersion").toString();
        }
        if (userJson.value("projectPath").isString()) {
            QDir directory(workingPath);
            QString directoryChange = userJson.value("projectPath").toString();
            directory.cd(directoryChange);
            projectPath = directory.absolutePath();
        }
        if (userJson.value("defaultCamera").isObject()) {
            QJsonObject defaultCamera = userJson.value("defaultCamera").toObject();
            if (defaultCamera.contains("cameraType")) {
                QString cameraType = defaultCamera.value("cameraType").toString();
                setCameraType(cameraType);
            }
            if (defaultCamera.value("speedMultiplier").isDouble()) {
                setSpeedMultiplier(defaultCamera.value("speedMultiplier").toDouble());
            }
        }
    }
}

void CProject::_loadEveryoneElse(QString directory) {
    QDirIterator directoryIt(directory);

    while(directoryIt.hasNext()) {
        directoryIt.next();

        QString dirName = directoryIt.fileInfo().fileName();
        QString dirPath = directoryIt.fileInfo().filePath();
        bool bIsDir = directoryIt.fileInfo().isDir();
        bool bIsFile = directoryIt.fileInfo().isDir();

        if (bIsDir) {
            if (!(dirName.compare(".") == 0 || dirName.compare("..") == 0)) {
                _loadEveryoneElse(dirPath);
            }
        }
        else {
            if (directoryIt.fileInfo().suffix().compare("json", Qt::CaseInsensitive) == 0) {
                _loadProjectObject(dirPath);
            }
        }
    }
}

void CProject::_loadProjectObject(QString userJsonFile) {
    QFile file(userJsonFile);
    if (file.open(QIODevice::ReadOnly)) {
        QString rawData = file.readAll();
        file.close();

        QJsonParseError jError;
        QJsonDocument document = QJsonDocument::fromJson(rawData.toUtf8(), &jError);

        if (jError.error == QJsonParseError::NoError) {
            QJsonObject userJsonObject = document.object();
            QString itemType = "";
            IProjectObject *newProjectObject = NULL;

            if (userJsonObject.contains("itemType")) {
                itemType = userJsonObject["itemType"].toString();
            }
            else {
                std::clog << "[WARNING]: Invalid project format. '" << userJsonFile.toStdString() << "'." << std::endl;
            }

            //
            // Start factory types.
            //

            if (itemType.compare("stage") == 0) {
                newProjectObject = new CStage(userJsonFile, userJsonObject);
            }
            else if (itemType.compare("framebuffer") == 0) {
                newProjectObject = new CFramebuffer(userJsonFile, userJsonObject);
            }
            else if (itemType.compare("project") == 0 || itemType.size() == 0) {
                // Just ignore this.
                return;
            }
            else {
                std::clog << "[WARNING]: Unrecognised type '" << itemType.toStdString() << "' found in file '" << userJsonFile.toStdString() << "'. Skipping." << std::endl;
            }

            //
            // End types.
            //

            if (newProjectObject != NULL) {
                projectList[newProjectObject->getItemName()] = newProjectObject;
            }

            return;
        }
        else {
            std::clog << "[WARNING]: JSON parse error. " << jError.errorString().toStdString() << ". In file '" << userJsonFile.toStdString() << "'." << std::endl;
        }

    }
    else {
        std::clog << "[WARNING]: Unable to open '" << userJsonFile.toStdString() <<"' for reading." << std::endl;
    }
}

bool CProject::openProject(QString projectFile) {
    closeProject();

    if (isJsonLoaded() == false) {
        bool bSuccess = loadUserJson(projectFile);
        if (bSuccess == false) {
            std::clog << "[ERROR]: Unable to load project type/name identifiers. Stopping." << std::endl;
            return false;
        }
    }

    _loadValuesFromUserJson();
    _loadEveryoneElse(projectPath);

    bool bResult = initialize();
    if (bResult == true) {
        std::clog << "[INFO] Project '" << projectFile.toStdString() << "' opened successfully." << std::endl;
    }
    else {
        std::clog << "[ERROR] Unable to open project '" << projectFile.toStdString() << "'." << std::endl;
    }

    return bResult;
}

bool CProject::closeProject() {
    QMap<QString, IProjectObject *>::iterator projectListIt = projectList.begin();
    for (; projectListIt != projectList.end(); ++projectListIt) {
        delete (*projectListIt);
    }
    projectList.clear();

    // Don't know why this is boolean.
    return true;
}

void CProject::run(float fFrameDelay) {

}


void CProject::setProjectVersion(QString projectVersion) {
    this->projectVersion = projectVersion;
}

void CProject::setCameraType(QString cameraType) {
    if (cameraType.compare("freeCamera") == 0) {
        this->cameraType = "freeCamera";
    }
    else if (cameraType.compare("orbitCamera") == 0) {
        this->cameraType = "orbitCamera";
    }
}

void CProject::setSpeedMultiplier(float fSpeedMultiplier) {
    if (fSpeedMultiplier > 1.0) {
        this->fSpeedMultiplier = fSpeedMultiplier;
    }
}

QString CProject::getProjectVersion() {
    return projectVersion;
}

QString CProject::getCameraType() {
    return cameraType;
}

float CProject::getSpeedMultiplier() {
    return fSpeedMultiplier;
}

bool CProject::initialize() {
    if (isJsonLoaded()) {
        this->bInitialized = true;

        setProjectList(&projectList);

        // Initialize other project objects.
        QMap<QString, IProjectObject *>::iterator projectListIt = projectList.begin();
        for (; projectListIt != projectList.end(); ++projectListIt) {
            IProjectObject *projectObject = (*projectListIt);
            bool bResult = projectObject->initialize();
            if (bResult == false) {
                std::clog << "[ERROR]: Initialization failed on " << projectObject->getItemType().toStdString() << "/" << projectObject->getItemName().toStdString() << "." << std::endl;
                this->bInitialized = false;
            }
        }

        // Initialize project object.
        if (userJson.value("stageList").isArray()) {
            QJsonArray userStageList = userJson.value("stageList").toArray();
            QJsonArray::iterator userStageListIt = userStageList.begin();
            for (; userStageListIt != userStageList.end(); ++userStageListIt) {
                if ((*userStageListIt).isString()) {
                    QString stage = (*userStageListIt).toString();
                    if (projectList.contains(stage)) {
                        stageList.push_back(projectList[stage]);
                    }
                    else {
                        std::clog << "[WARNING]: Specified stage '" << stage.toStdString() << "' not found." << std::endl;
                    }
                }
            }
        }

        if (stageList.size() == 0) {
            this->bInitialized = false;
        }

        if (this->bInitialized == false) {
            closeProject();
        }
        return this->bInitialized;
    }

    std::clog << "[ERROR]: Project type/name unspecified." << std::endl;
    closeProject();
    return false;
}

void CProject::setViewPort(int iWidth, int iHeight) {
    QMap<QString, IProjectObject *>::iterator projectListIt = projectList.begin();
    for (; projectListIt != projectList.end(); ++projectListIt) {
        (*projectListIt)->setViewPort(iWidth, iHeight);
    }
    viewPortWidth = iWidth;
    viewPortHeight = iHeight;
}

void CProject::setCamera(CCamera *camera) {
    QMap<QString, IProjectObject *>::iterator projectListIt = projectList.begin();
    for (; projectListIt != projectList.end(); ++projectListIt) {
        (*projectListIt)->setCamera(camera);
    }
    this->camera = camera;
}

void CProject::setProjectList(QMap<QString, IProjectObject *> *lpProjectList) {
    QMap<QString, IProjectObject *>::iterator projectListIt = projectList.begin();
    for (; projectListIt != projectList.end(); ++projectListIt) {
        (*projectListIt)->setProjectList(lpProjectList);
    }
    this->lpProjectList = lpProjectList;
}
