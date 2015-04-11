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

    projectionMatrix = glm::perspective(fovy, fAspect, zNear, zFar);
    bChanged = false;
}

CPerspective::~CPerspective()
{

}

glm::mat4 CPerspective::generateProjectionMatrix(int iWidth, int iHeight) {
    bChanged = this->iWidth != iWidth ? true : bChanged;
    bChanged = this->iHeight != iHeight ? true : bChanged;

    if (bChanged) {
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
    // Assume it was changed by the caller.
    bChanged = true;
    return &aspect;
}
