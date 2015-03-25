#ifndef CSTAGE_H
#define CSTAGE_H

#include "IProjectObject.h"
#include "glm/glm.hpp"

class CStage: public IProjectObject {
private:
    glm::mat4 projectionMatrix;
    QList<IProjectObject *> framebufferList;
    QList<IProjectObject *> modelList;
    QList<IProjectObject *> textureList;

public:
    CStage();
    ~CStage();

    // bool loadUserJson(QJsonObject userJson);
    bool initiaize();
    bool run(float elapsedTime);

public:
    glm::mat4 getProjectionMatrix();
};

#endif // CSTAGE_H

