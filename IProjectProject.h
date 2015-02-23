#ifndef IPROJECTPROJECT_H
#define IPROJECTPROJECT_H

#include "IProjectObject.h"

enum EProjectDefalutCamera {
    DEFAULT_CAMERA_ORBIT,
    DEFAULT_CAMERA_FREE
};

class IProjectProject : public IProjectObject {
public:
    IProjectProject() {}
    ~IProjectProject() {}

    // bool loadUserJson(QJsonObject userJson);
    virtual bool initiaize() = 0;
    virtual bool run(float elapsedTime) = 0;

public:
    virtual float getSpeedMultiplier() = 0;
    virtual EProjectDefalutCamera getDefaultCamera() = 0;
    virtual QString getProjectPath() = 0;
    virtual QString getProjectVersion() = 0;
};

#endif // IPROJECTPROJECT_H

