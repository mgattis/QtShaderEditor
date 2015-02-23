#include "CProjectProject.h"
#include "CProjectStage.h"

#include <QJsonParseError>

CProjectProject::CProjectProject() {
    projectPath = QDir::home();
    projectVersion = "v0.1b";

    defaultCamera = DEFAULT_CAMERA_ORBIT;
    speedMultiplier = 1.0;
}

CProjectProject::~CProjectProject() {

}

QJsonObject CProjectProject::_loadJsonFile(QString userJsonFile) {
    QJsonObject json;

    std::clog << "[INFO]: Opening '" << userJsonFile.toStdString() << "' for reading." << std::endl;

    QFile file(userJsonFile);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (file.isOpen()) {
        QString rawData = file.readAll();
        file.close();

        QJsonParseError jError;
        QJsonDocument document = QJsonDocument::fromJson(rawData.toUtf8(), &jError);

        if (jError.error != QJsonParseError::NoError) {
            std::clog << "[WARNING]: JSON parse error: '" << jError.errorString().toStdString() << "'" << std::endl;
            return json;
        }

        json = document.object();
    }
    else {
        std::clog << "[WARNING]: Unable to open '" << userJsonFile.toStdString() << "' for reading." << std::endl;
    }

    return json;
}

bool CProjectProject::loadProject(QString userJsonFile) {
    userJsonFile = QDir(userJsonFile).absolutePath();
    QFileInfo fileInfo = userJsonFile;
    projectPath = fileInfo.absoluteDir();

    QJsonObject userJson = _loadJsonFile(userJsonFile);
    bool bSuccess = loadUserJson(userJson);
    if (bSuccess && !this->itemType.compare("project")) {
        std::clog << "[INFO]: '" << this->itemName.toStdString() << "'." << std::endl;

        CResourceManager *resourceManager = CResourceManager::instance();
        resourceManager->clearLists();
        resourceManager->addProjectObject(this);

        return initiaize();
    }
    else {
        std::clog << "[ERROR]: Unable to load project." << std::endl;
    }

    return false;
}

bool CProjectProject::initiaize() {
    // Only if the project JSON is loaded.
    if (this->bIdentifiersLoaded) {
        if (userJson.contains("projectVersion")) {
            this->projectVersion = userJson["projectVersion"].toString();
        }
        if (userJson.contains("projectPath")) {
            // Change directory.
            this->projectPath.cd(userJson["projectPath"].toString());
        }
        if (userJson.contains("defaultCamera")) {
            QJsonObject defaultCamera = userJson["defaultCamera"].toObject();
            if (defaultCamera.contains("cameraType")) {
                if (!defaultCamera["cameraType"].toString().compare("freeCamera")) {
                    this->defaultCamera = DEFAULT_CAMERA_FREE;
                }
                if (defaultCamera.contains("speedMultiplier")) {
                    this->speedMultiplier = defaultCamera["speedMultiplier"].toDouble();
                }
            }
        }

        // Load everyone else.
        bool bSuccess = _loadAllProjectObjects(projectPath);

        if (bSuccess) {
            // Load all stages.
            if (userJson.contains("stageList")) {
                CResourceManager *resourceManager = CResourceManager::instance();
                QJsonArray stageList = userJson["stageList"].toArray();

                if (stageList.size()) {
                    QJsonArray::const_iterator stageListIt = stageList.begin();
                    for (; stageListIt != stageList.end(); ++stageListIt) {
                        QString stageName = (*stageListIt).toString();

                        IProjectStage *stage = resourceManager->getProjectObject(stageName);
                        this->stageList.push_back(stage);
                    }
                }
            }

            // Link everything together.
            bSuccess = _initializeAllProjectObjects();

            bInitializeSuccess = bSuccess;
            return bSuccess;
        }
    }

    bInitializeSuccess = false;
    return false;
}

bool CProjectProject::_loadAllProjectObjects(QDir directory) {
    QDirIterator directoryIt(directory);

    while (directoryIt.hasNext()) {
        directoryIt.next();

        QString dirName = directoryIt.fileInfo().fileName();
        QString dirPath = directoryIt.fileInfo().filePath();
        bool bIsDir = directoryIt.fileInfo().isDir();
        bool bIsFile = directoryIt.fileInfo().isFile();

        if (bIsDir) {
            if (!(dirName.compare(".") == 0 || dirName.compare("..") == 0)) {
                _loadAllProjectObjects(QDir(dirPath));
            }
        }
        else if (bIsFile) {
            if (!directoryIt.fileInfo().suffix().compare("json")) {
                // Load the project file and create the associated object.
                // If the type is 'project' it will be ignored in '_loadProjectObject()'.
                QJsonObject userJson = _loadJsonFile(dirPath);
                _loadProjectObject(userJson);
            }
        }
    }

    return true;
}

bool CProjectProject::_loadProjectObject(QJsonObject userJson) {
    if (userJson.contains("itemType")) {
        IProjectObject *projectObject = NULL;
        QString itemType = userJson["itemType"].toString();

        // Determine type and create the right object.
        if (itemType.compare("stage") == 0) {
            projectObject = new CProjectStage();
        }
        // End types.

        // Add the newly created object.
        if (projectObject) {
            bool bSuccess = projectObject->loadUserJson(userJson);
            if (bSuccess) {
                projectObject->setProjectPath(this->projectPath);
                CResourceManager::instance()->addProjectObject(projectObject);

                return true;
            }
        }

        return false;
    }

    return false;
}

bool CProjectProject::_initializeAllProjectObjects() {
    QMap<QString, IProjectObject *> projectList;
    bool bSuccess = true;

    projectList = CResourceManager::instance()->getAllProjectObjects();

    QMap<QString, IProjectObject *>::const_iterator projectListIt = projectList.begin();
    for (; projectListIt != projectList.end(); ++projectListIt) {
        // Initialize all project objects except the project object its self.
        if ((*projectListIt)->getType().compare("project")) {
            bSuccess = (*projectListIt)->initiaize();

            if (!bSuccess) {
                return false;
            }
        }
    }

    return true;
}

bool CProjectProject::run(float elapsedTime) {
    QList<IProjectStage *>::const_iterator stageListIt = stageList.begin();
    for (; stageListIt != stageList.end(); ++stageListIt) {
        IProjectStage *stage = (*stageListIt);

        stage->run(elapsedTime);
    }

    return false;
}

float CProjectProject::getSpeedMultiplier() {
    return speedMultiplier;
}

EProjectDefalutCamera CProjectProject::getDefaultCamera() {
    return defaultCamera;
}

QString CProjectProject::getProjectPath() {
    return projectPath.canonicalPath();
}

QString CProjectProject::getProjectVersion() {
    return projectVersion;
}
