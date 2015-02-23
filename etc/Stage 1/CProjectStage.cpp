#include "CProjectStage.h"
#include "glm/glm.hpp"

CProjectStage::CProjectStage() {
    this->projectionMatrix = glm::mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
}

CProjectStage::~CProjectStage() {

}

bool CProjectStage::initiaize() {

	return false;
}

bool CProjectStage::run(float elapsedTime) {

	return false;
}

glm::mat4 CProjectStage::getProjectionMatrix() {
    return this->projectionMatrix;
}
