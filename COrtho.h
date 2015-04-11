#ifndef CORTHO_H
#define CORTHO_H

#include "CProjectionMatrix.h"

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
    COrtho(float left, float right, float bottom, float top, float zNear, float zFar);
    ~COrtho();

    glm::mat4 generateProjectionMatrix(int iWidth, int iHeight);

    void setAll(float left, float right, float bottom, float top, float zNear, float zFar);
    void setLeft(float left);
    void setRight(float right);
    void setBottom(float bottom);
    void setTop(float top);
    void setZNear(float zNear);
    void setZFar(float zFar);
};

#endif // CORTHO_H
