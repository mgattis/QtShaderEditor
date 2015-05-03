#include "CProject.h"

#include <QJsonParseError>

CProject::CProject() {
    projectVersion = "v0.1b";
    cameraType = "freeCamera";
    fSpeedMultiplier = 1.0;
    lpProjectList = &projectList;

    // Total project run time.
    bRunTimeListPopulated = false;
    fRunTime = 0.0;

    bHasValidProject = false;
}

CProject::CProject(QString projectFile) : IProjectObject(projectFile) {
    projectVersion = "v0.1b";
    cameraType = "freeCamera";
    fSpeedMultiplier = 1.0;
    lpProjectList = &projectList;

    // Total project run time.
    bRunTimeListPopulated = false;
    fRunTime = 0.0;

    bHasValidProject = false;

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
        bool bIsFile = directoryIt.fileInfo().isFile();

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

            if (userJsonObject.value("itemType").isString()) {
                itemType = userJsonObject["itemType"].toString();
            }
            else {
				QJsonValue value = userJsonObject.value("itemType");
				value.isString();
                logWarning(QString("Invalid project format. '") + userJsonFile + QString("'."));
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
            else if (itemType.compare("texture") == 0) {
                newProjectObject = new CTexture(userJsonFile, userJsonObject);
            }
            else if (itemType.compare("shader") == 0) {
                newProjectObject = new CShader(userJsonFile, userJsonObject);
            }
            else if (itemType.compare("model") == 0) {
                newProjectObject = new CModel(userJsonFile, userJsonObject);
            }
            else if (itemType.compare("project") == 0 || itemType.size() == 0) {
                // Just ignore this.
                return;
            }
            else {
                logWarning(QString("Skipping unrecognised type: '") + itemType + QString("' in file '") + userJsonFile + "'.");
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
            logWarning(QString("JSON parse error '") + jError.errorString() + QString("' in file '") + userJsonFile + QString("'."));
        }

    }
    else {
        logWarning(QString("Unable to open project file '") +  userJsonFile + QString("' for reading."));
    }
}

bool CProject::openProject(QString projectFile) {
    closeProject();

    if (isJsonLoaded() == false) {
        bool bSuccess = loadUserJson(projectFile);
        if (bSuccess == false) {
            logError("Unable to load project identifiers. Stopping.");
            return false;
        }
    }

    _loadValuesFromUserJson();
    _loadEveryoneElse(projectPath);

    bool bResult = initialize();
    if (bResult == true) {
        logInfo(QString("Project opened successfully. '") + projectFile + QString("'."));
    }
    else {
        logError(QString("Unable to open project '") + projectFile + QString("'."));
    }

    bHasValidProject = bResult;
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
    fRunTime += fFrameDelay;
    setRunTime(fRunTime);

    QList<CStage *>::iterator stageListIt = stageList.begin();
    for (;stageListIt != stageList.end(); ++stageListIt) {
        CStage *stage = (*stageListIt);
        stage->run();
    }
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
                logError(QString("Initialization failed. '") + projectObject->getFullIdentifier() + QString("'."));
                this->bInitialized = false;
                return false;
            }
        }

        // We will take a second step to load all models because they need
        // valid shaders
        projectListIt = projectList.begin();
        for (; projectListIt != projectList.end(); ++projectListIt) {
            IProjectObject *projectObject = (*projectListIt);
            if (projectObject->getItemType().compare("model") == 0) {
                CModel *model = (CModel *)projectObject;
                if (model->getDrawShader()) {
                    model->loadModel();
                }
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
                        logWarning(QString("Stage not found. '") + stage + QString("'."));
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

    logError("Project identifiers not loaded.");
    closeProject();
    return false;
}

bool CProject::hasValidProject() {
    return bHasValidProject;
}

void CProject::setViewPort(int iWidth, int iHeight) {
    QMap<QString, IProjectObject *>::iterator projectListIt = projectList.begin();
    for (; projectListIt != projectList.end(); ++projectListIt) {
        (*projectListIt)->setViewPort(iWidth, iHeight);
    }
    iViewPortWidth = iWidth;
    iViewPortHeight = iHeight;
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

void CProject::setRunTime(float fRunTime) {
    if (bRunTimeListPopulated == false) {
        QMap<QString, IProjectObject *>::iterator projectListIt = projectList.begin();
        for (; projectListIt != projectList.end(); ++projectListIt) {
            IProjectObject *projectObject = (*projectListIt);
            if (projectObject->getItemType().compare("shader") == 0) {
                shaderList.push_back((CShader *)projectObject);
            }
        }

        bRunTimeListPopulated = true;
    }

    QList<CShader *>::iterator shaderListIt = shaderList.begin();
    for (; shaderListIt != shaderList.end(); ++shaderListIt) {
        (*shaderListIt)->setRunTime(fRunTime);
    }
}
