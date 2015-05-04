#ifndef CPROJECTIONMATRIX_H
#define CPROJECTIONMATRIX_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QString>
#include <QJsonObject>

class CProjectionMatrix {
protected:
    glm::mat4 projectionMatrix;
    bool bChanged;

public:
    CProjectionMatrix();
	virtual ~CProjectionMatrix();

    // Updates the projection matrix for the given view port.
    virtual glm::mat4 getProjectionMatrix(int iWidth, int iHeight) = 0;

    // Grabs the matrix for the last supplied viewport.
    virtual glm::mat4 getProjectionMatrix() = 0;

    // Type of matrix.
    virtual QString getType();

    // Loads user JSON parameters
    virtual void userJsonProjection(QJsonObject projection) = 0;
};

#endif // CPROJECTIONMATRIX_H
