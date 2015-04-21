#ifndef CPERSPECTIVE_H
#define CPERSPECTIVE_H

#include "CProjectionMatrix.h"
#include "CAspectRatio.h"

class CPerspective : public CProjectionMatrix {
public:
    float fovy;
    CAspectRatio aspect;
    float zNear;
    float zFar;

    int iWidth;
    int iHeight;

public:
    CPerspective();
    ~CPerspective();

    glm::mat4 getProjectionMatrix(int iWidth, int iHeight);
    glm::mat4 getProjectionMatrix();
    QString getType();

    void setfovy(float fovy);
    void setzNear(float zNear);
    void setzFar(float zFar);
    CAspectRatio *getAspectRatio();
};

#endif // CPERSPECTIVE_H
