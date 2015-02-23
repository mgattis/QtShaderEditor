#ifndef IPROJECTSTAGE_H
#define IPROJECTSTAGE_H

#include "IProjectObject.h"

#include <iostream>
#include <QString>
#include <QJsonObject>
#include <glm/glm.hpp>

class IProjectStage: public IProjectObject {
protected:
    glm::mat4 projectionMatrix;
    QList<IProjectObject *> framebufferList;
    QList<IProjectObject *> modelList;
    QList<IProjectObject *> textureList;

public:
    IProjectStage() {}
    ~IProjectStage() {}

    // bool loadUserJson(QJsonObject userJson);
    virtual bool initiaize() = 0;
    virtual bool run(float elapsedTime) = 0;

public:
    virtual glm::mat4 getProjectionMatrix() = 0;
};

#endif // IPROJECTSTAGE_H

