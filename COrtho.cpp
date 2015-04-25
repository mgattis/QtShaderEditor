#include "COrtho.h"

COrtho::COrtho()
{
    fLeft = -1.0;
    fRight = 1.0;
    fBottom = -1.0;
    fTop = 1.0;
    zNear = -1.0;
    zFar = 1.0;
    projectionMatrix = glm::mat4();
    bChanged = true;
}

COrtho::~COrtho()
{

}

glm::mat4 COrtho::getProjectionMatrix(int iWidth, int iHeight) {
    return getProjectionMatrix();
}

glm::mat4 COrtho::getProjectionMatrix() {
    // We don't need the viewport for an ortho matrix.

    if (bChanged) {
        projectionMatrix = glm::ortho(fLeft, fRight, fBottom, fTop, zNear, zFar);
        bChanged = false;
        return projectionMatrix;
    }
    else {
        return projectionMatrix;
    }
}

QString COrtho::getType() {
    return QString("ortho");
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

void COrtho::userJsonProjection(QJsonObject projection) {
    if (projection.value("left").isDouble()) {
        setLeft(projection.value("left").toDouble());
    }
    if (projection.value("right").isDouble()) {
        setRight(projection.value("right").toDouble());
    }
    if (projection.value("bottom").isDouble()) {
        setBottom(projection.value("bottom").toDouble());
    }
    if (projection.value("top").isDouble()) {
        setTop(projection.value("top").toDouble());
    }
    if (projection.value("nearVal").isDouble()) {
        setZNear(projection.value("nearVal").toDouble());
    }
    if (projection.value("farVal").isDouble()) {
        setZFar(projection.value("farVal").toDouble());
    }
}
