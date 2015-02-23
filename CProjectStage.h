#ifndef CPROJECTSTAGE_H
#define CPROJECTSTAGE_H

#include "IProjectStage.h"

class CProjectStage: public IProjectStage {
private:
    glm::mat4 projectionMatrix;
    QList<IProjectObject *> framebufferList;
    QList<IProjectObject *> modelList;
    QList<IProjectObject *> textureList;

public:
    CProjectStage();
    ~CProjectStage();

    // bool loadUserJson(QJsonObject userJson);
    bool initiaize();
    bool run(float elapsedTime);

public:
    glm::mat4 getProjectionMatrix();
};

#endif // CPROJECTSTAGE_H

