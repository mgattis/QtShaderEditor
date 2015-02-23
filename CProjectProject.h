#ifndef CPROJECTPROJECT_H
#define CPROJECTPROJECT_H

#include "IProjectProject.h"
#include "IProjectStage.h"
#include "CJsonTemplate.h"
#include "CPath.h"
#include "CResourceManager.h"

#include <QJsonDocument>
#include <QByteArray>
#include <QList>
#include <QDir>
#include <QDirIterator>
#include <QFile>

class CProjectProject : public IProjectProject {
private:
    QList<IProjectStage *> stageList;

    QString projectVersion;

    EProjectDefalutCamera defaultCamera;
    float speedMultiplier;

private:
    QJsonObject _loadJsonFile(QString userJson);
    bool _loadProjectObject(QJsonObject userJson);
    bool _loadAllProjectObjects(QDir directory);
    bool _initializeAllProjectObjects();

public:
    CProjectProject();
    ~CProjectProject();

    // bool loadUserJson(QJsonObject userJson);
    bool loadProject(QString userJsonFile);

    bool initiaize();
    bool run(float elapsedTime);

    float getSpeedMultiplier();
    EProjectDefalutCamera getDefaultCamera();
    QString getProjectPath();
    QString getProjectVersion();
};

#endif /* !CPROJECTPROJECT_H */
