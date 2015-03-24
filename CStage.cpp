#include "CStage.h"

CStage::CStage() {
    this->projectionMatrix = glm::mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
}

CStage::~CStage() {

}

bool CStage::initiaize() {

	return false;
}

bool CStage::run(float elapsedTime) {

	return false;
}

glm::mat4 CStage::getProjectionMatrix() {
    return this->projectionMatrix;
}
