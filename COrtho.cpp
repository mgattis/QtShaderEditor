#include "COrtho.h"

COrtho::COrtho()
{
    fLeft = -1.0;
    fRight = 1.0;
    fBottom = -1.0;
    fTop = 1.0;
    zNear = -1.0;
    zFar = 1.0;
    projectionMatrix = glm::ortho(fLeft, fRight, fBottom, fTop, zNear, zFar);
    bChanged = false;
}

COrtho::~COrtho()
{

}

glm::mat4 COrtho::generateProjectionMatrix(int iWidth, int iHeight) {
    // We don't need width and height for Ortho matrix;

    if (bChanged) {
        bChanged = false;
        projectionMatrix = glm::ortho(fLeft, fRight, fBottom, fTop, zNear, zFar);
        return projectionMatrix;
    }
    else {
        return projectionMatrix;
    }
}

void COrtho::setAll(float left, float right, float bottom, float top, float zNear, float zFar) {
    fLeft = left;
    fRight = right;
    fBottom = bottom;
    fTop = top;
    this->zNear = zNear;
    this->zFar = zFar;
    bChanged = true;
}

void COrtho::setLeft(float left) {
    fLeft = left;
    bChanged = true;
}

void COrtho::setRight(float right) {
    fRight = right;
    bChanged = true;
}

void COrtho::setBottom(float bottom) {
    fBottom = bottom;
    bChanged = true;
}

void COrtho::setTop(float top) {
    fTop = top;
    bChanged = true;
}

void COrtho::setZNear(float zNear) {
    this->zNear = zNear;
    bChanged = true;
}

void COrtho::setZFar(float zFar) {
    this->zFar = zFar;
    bChanged = true;
}
