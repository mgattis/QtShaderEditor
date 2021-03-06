#ifndef CORTHO_H
#define CORTHO_H

#include "CProjectionMatrix.h"

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/glm.hpp>

class COrtho : public CProjectionMatrix {
private:
    float fLeft;
    float fRight;
    float fBottom;
    float fTop;
    float zNear;
    float zFar;

public:
    COrtho();
    ~COrtho();

    glm::mat4 getProjectionMatrix(int iWidth, int iHeight);
    glm::mat4 getProjectionMatrix();
    QString getType();

    void setLeft(float left);
    void setRight(float right);
    void setBottom(float bottom);
    void setTop(float top);
    void setZNear(float zNear);
    void setZFar(float zFar);

    void userJsonProjection(QJsonObject projection);
};

#endif // CORTHO_H
