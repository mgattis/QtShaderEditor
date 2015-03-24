#ifndef CCAMERA_H
#define CCAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <GL/glew.h>

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
};

#endif // CCAMERA_H
