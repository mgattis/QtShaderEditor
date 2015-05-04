#include "CProjectionMatrix.h"

CProjectionMatrix::CProjectionMatrix() {
    projectionMatrix = glm::mat4();
    bChanged = false;
}

CProjectionMatrix::~CProjectionMatrix() {
}

QString CProjectionMatrix::getType() {
    return QString("projectionMatrix");
}
