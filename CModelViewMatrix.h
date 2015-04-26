#ifndef CMODELVIEWMATRIX_H
#define CMODELVIEWMATRIX_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QJsonArray>
#include <QJsonObject>

#ifndef M_PI
#define M_PI (3.1415926535897932384626433832795)
#endif

class CModelViewMatrix {
private:
    glm::mat4 matrix;
    bool bLoadIdentity;

public:
    CModelViewMatrix();
    CModelViewMatrix(glm::mat4 matrix);
    CModelViewMatrix(glm::mat4 matrix, QJsonArray transforms);
    ~CModelViewMatrix();

    void setMatrix(glm::mat4 matrix);
    glm::mat4 getMatrix();

    // The real convienence method.
    void userJsonTransform(QJsonArray transforms);

    glm::mat4 loadIdentity();
    glm::mat4 rotate(float angle, float x, float y, float z);
    glm::mat4 translate(float x, float y, float z);
    glm::mat4 scale(float x, float y, float z);

    bool identityFlag();
    void setIdentityFlag();
    void clearIdentityFlag();
};

#endif // CMODELVIEWMATRIX_H
