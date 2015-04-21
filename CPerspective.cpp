#include "CPerspective.h"

CPerspective::CPerspective()
{
    fovy = 80.0;
    aspect.setWindowWidth(1.0);
    aspect.setWindowHeight(1.0);
    zNear = 0.1;
    zFar = 80.0;

    iWidth = 800;
    iHeight = 600;

    float fAspect = aspect.getAspectRatio(800, 600);

    projectionMatrix = glm::mat4();
    bChanged = true;
}

CPerspective::~CPerspective()
{

}

glm::mat4 CPerspective::getProjectionMatrix(int iWidth, int iHeight) {
    bChanged = this->iWidth != iWidth ? true : bChanged;
    bChanged = this->iHeight != iHeight ? true : bChanged;

    if (bChanged || aspect.isAspectChanged()) {
        float fAspect = aspect.getAspectRatio(iWidth, iHeight);
        this->iWidth = iWidth;
        this->iHeight = iHeight;
        bChanged = false;

        projectionMatrix = glm::perspective(fovy, fAspect, zNear, zFar);
        return projectionMatrix;
    }
    else {
        return projectionMatrix;
    }
}

glm::mat4 CPerspective::getProjectionMatrix() {
    if (bChanged) {
        float fAspect = aspect.getAspectRatio(iWidth, iHeight);
        bChanged = false;

        projectionMatrix = glm::perspective(fovy, fAspect, zNear, zFar);
        return projectionMatrix;
    }
    else {
        return projectionMatrix;
    }
}

QString CPerspective::getType() {
    return QString("perspective");
}

void CPerspective::setfovy(float fovy) {
    this->fovy = fovy;
    bChanged = true;
}

void CPerspective::setzNear(float zNear) {
    this->zNear = zNear;
    bChanged = true;
}

void CPerspective::setzFar(float zFar) {
    this->zFar = zFar;
    bChanged = true;
}

CAspectRatio *CPerspective::getAspectRatio() {
    // No longer assume it was changed. We will check for changes in getProjectionMatrix();
    return &aspect;
}
