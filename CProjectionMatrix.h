#ifndef CPROJECTIONMATRIX_H
#define CPROJECTIONMATRIX_H

#include <glm/gtc/matrix_transform.hpp>

class CProjectionMatrix {
protected:
    glm::mat4 projectionMatrix;
    bool bChanged;

public:
    CProjectionMatrix();
    virtual glm::mat4 generateProjectionMatrix(int iWidth, int iHeight) = 0;
};

#endif // CPROJECTIONMATRIX_H
