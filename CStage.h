#ifndef CSTAGE_H
#define CSTAGE_H

#include "CResourceManager.h"
#include "IProjectObject.h"
#include "glm/glm.hpp"

enum ProjectionType_t {
    PROJECTION_ORTO,
    PROJECTION_PERSPECTIVE
};

class CStage: public IProjectObject {
private:
    glm::mat4 projectionMatrix;
    QList<IProjectObject *> inputFrameBuffers;
    QList<IProjectObject *> modelList;

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

