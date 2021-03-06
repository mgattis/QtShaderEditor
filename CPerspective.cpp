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

	aspect.getAspectRatio(800, 600);

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
    if (bChanged || aspect.isAspectChanged()) {
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
    return &aspect;
}

void CPerspective::userJsonProjection(QJsonObject projection) {
    if (projection.value("fovy").isDouble()) {
        setfovy(projection.value("fovy").toDouble());
    }
    if (projection.value("aspect").isObject()) {
        QJsonObject userAspect = projection.value("aspect").toObject();
        if (userAspect.value("windowWidth").isDouble()) {
            aspect.setWindowWidth(userAspect.value("windowWidth").toDouble());
        }
        else if (userAspect.value("pixelWidth").isDouble()) {
            aspect.setPixelWidth(userAspect.value("pixelWidth").toDouble());
        }
        if (userAspect.value("windowHeight").isDouble()) {
            aspect.setWindowHeight(userAspect.value("windowHeight").toDouble());
        }
        else if (userAspect.value("pixelHeight").toDouble()) {
            aspect.setPixelHeight(userAspect.value("pixelHeight").toDouble());
        }
        if (userAspect.value("multiplier").isDouble()) {
            aspect.setMultiplier(userAspect.value("multiplier").toDouble());
        }
    }
    if (projection.value("zNear").isDouble()) {
        setzNear(projection.value("zNear").toDouble());
    }
    if (projection.value("zFar").isDouble()) {
        setzFar(projection.value("zFar").toDouble());
    }
}
