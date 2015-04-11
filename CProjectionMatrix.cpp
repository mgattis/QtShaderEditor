#include "CProjectionMatrix.h"

CProjectionMatrix::CProjectionMatrix() {
    projectionMatrix = glm::mat4();
    bChanged = false;
}
