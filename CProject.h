#ifndef CPROJECT_H
#define CPROJECT_H

#include "IProjectObject.h"
#include "CStage.h"
#include "CFramebuffer.h"
#include "CShader.h"
#include "CModel.h"
#include "CTexture.h"
#include "CPath.h"

#include <QDir>
#include <QDirIterator>
#include <QJsonArray>

class CProject : public IProjectObject {
private:
    // Whole project stuffs.
    QMap<QString, IProjectObject *> projectList;

private:
    // User JSON stuffs.
    QList<CStage *> stageList;
    QString projectVersion;
    QString projectPath;
    QString cameraType;
    float fSpeedMultiplier;

private:
    void _loadValuesFromUserJson();
    void _loadEveryoneElse(QString directory);
    void _loadProjectObject(QString userJsonFile);

public:
    CProject();
    CProject(QString projectFile);
    ~CProject();

    bool openProject(QString projectFile);
    bool closeProject();
    void run(float fFrameDelay);

    void setProjectVersion(QString projectVersion);
    void setCameraType(QString cameraType);
    void setSpeedMultiplier(float fSpeedMultiplier);

    QString getProjectVersion();
    QString getCameraType();
    float getSpeedMultiplier();

public:
    bool initialize();

    void setViewPort(int iWidth, int iHeight);
    void setCamera(CCamera *camera);
    void setProjectList(QMap<QString, IProjectObject *> *lpProjectList);
};

#endif /* !CPROJECT_H */
