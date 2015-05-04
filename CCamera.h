#ifndef CCAMERA_H
#define CCAMERA_H

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <GL/glew.h>

#ifndef M_PI
#define M_PI (3.1415926535897932384626433832795)
#endif

class CCamera {
public:
    glm::vec4 position;
    glm::vec4 velocity;
    glm::vec4 angle;

    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 modelMatrix;

public:
    CCamera();
    ~CCamera();

    glm::mat4 getMatrixFromPosition();
    void resetCamera();
};

#endif // CCAMERA_H
