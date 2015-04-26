#include "CCamera.h"

#ifndef M_PI
#define M_PI (3.14159265359)
#endif

CCamera::CCamera() {
    projectionMatrix = glm::mat4();
    viewMatrix = glm::mat4();
    modelMatrix = glm::mat4();

    position = glm::vec4(0.0, 0.0, 0.0, 0.0);
    velocity = glm::vec4(0.0, 0.0, 0.0, 0.0);
    angle = glm::vec4(0.0, 0.0, 0.0, 0.0);
}

CCamera::~CCamera() {

}

glm::mat4 CCamera::getMatrixFromPosition() {
    /*while (angle.z < 0.0) {
        angle.z += 360.0;
    }
    while (angle.z >= 360.0) {
        angle.z -= 360.0;
    }*/

#if 0
	glm::mat4 m = glm::rotate(glm::mat4(), (float)(angle.x), glm::vec3(1.0, 0.0, 0.0));
	m = glm::rotate(m, (float)(angle.y), glm::vec3(0.0, 1.0, 0.0));
#endif
    glm::mat4 m = glm::rotate(glm::mat4(), (float)(angle.x), glm::vec3(1.0, 0.0, 0.0));
    m = glm::rotate(m, (float)(angle.z), glm::vec3(0.0, 0.0, 1.0));
    m = glm::translate(m, glm::vec3(position));
    return m;
}
