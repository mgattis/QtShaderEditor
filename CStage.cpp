#include "CStage.h"

CStage::CStage() {
    this->projectionMatrix = glm::mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
}

CStage::~CStage() {

}

bool CStage::initiaize() {
#if 0
    if (this->bIdentifiersLoaded) {
        if (userJson.contains("generateProjectionMatrixPerspective")) {
            projectionType = PROJECTION_PERSPECTIVE;
            QJsonObject projection = userJson["generateProjectionMatrixPerspective"];
            if (projection.contains("fovy")) {

            }
        }
    }
#endif
	return false;
}

bool CStage::run(float elapsedTime) {

	return false;
}

glm::mat4 CStage::getProjectionMatrix() {
    return this->projectionMatrix;
}
