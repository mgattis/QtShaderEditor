#ifndef CPROJECT_H
#define CPROJECT_H

#include "CProject.h"
#include "CStage.h"
#include "CJsonTemplate.h"
#include "CPath.h"
#include "CResourceManager.h"

#include <QJsonDocument>
#include <QByteArray>
#include <QList>
#include <QDir>
#include <QDirIterator>
#include <QFile>

enum CameraType_t {
    CAMERA_ORBIT,
    CAMERA_FREE
};

class CProject : public IProjectObject {
private:
    QList<CStage *> stageList;
    QString projectVersion;
    CameraType_t defaultCamera;
    float speedMultiplier;

private:
    QJsonObject _loadJsonFile(QString userJson);
    bool _loadProjectObject(QJsonObject userJson);
    bool _loadAllProjectObjects(QDir directory);
    bool _initializeAllProjectObjects();

public:
    CProject();
    ~CProject();

    // bool loadUserJson(QJsonObject userJson);
    bool loadProject(QString userJsonFile);
    void setViewPort(int iWidth, int iHeight);

    bool initiaize();
    bool run(float elapsedTime);

    float getSpeedMultiplier();
    CameraType_t getDefaultCamera();
    QString getProjectPath();
    QString getProjectVersion();
};

#endif /* !CPROJECT_H */
