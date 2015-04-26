#include "CModelViewMatrix.h"

CModelViewMatrix::CModelViewMatrix()
{
    matrix = glm::mat4();
    bLoadIdentity = false;
}

CModelViewMatrix::CModelViewMatrix(glm::mat4 matrix) {
    this->matrix = matrix;
    bLoadIdentity = false;
}

CModelViewMatrix::~CModelViewMatrix()
{

}

void CModelViewMatrix::setMatrix(glm::mat4 matrix) {
    this->matrix = matrix;
}

glm::mat4 CModelViewMatrix::getMatrix() {
    return matrix;
}

// Applies transform list from user JSON.
void CModelViewMatrix::userJsonTransform(QJsonArray transforms) {
    QJsonArray::iterator transformsIt = transforms.begin();
    for (; transformsIt != transforms.end(); ++transformsIt) {
        if ((*transformsIt).isObject()) {
            QJsonObject transformObj = (*transformsIt).toObject();

            if (transformObj.value("loadIdentity").isBool()) {
                if (transformObj.value("loadIdentity").toBool() == true) {
                    loadIdentity();
                }
            }
            else if (transformObj.value("rotate").isArray()) {
                QJsonArray rotateParamsArray = transformObj.value("rotate").toArray();
                QJsonArray::iterator rotateParamsArrayIt = rotateParamsArray.begin();
                float rotateParams[] = {0.0, 0.0, 0.0, 0.0};
                int i = 0;
                for (; rotateParamsArrayIt != rotateParamsArray.end(); ++rotateParamsArrayIt) {
                    if ((*rotateParamsArrayIt).isDouble()) {
                        rotateParams[i] = (*rotateParamsArrayIt).toDouble();
                    }
                    i++;
                    if (i >= 4) {
                        break;
                    }
                }
                rotate(rotateParams[0], rotateParams[1], rotateParams[2], rotateParams[3]);
            }
            else if (transformObj.value("translate").isArray()) {
                QJsonArray translateParamsArray = transformObj.value("translate").toArray();
                QJsonArray::iterator translateParamsArrayIt = translateParamsArray.begin();
                float translateParams[] = {0.0, 0.0, 0.0};
                int i = 0;
                for (; translateParamsArrayIt != translateParamsArray.end(); ++translateParamsArrayIt) {
                    if ((*translateParamsArrayIt).isDouble()) {
                        translateParams[i] = (*translateParamsArrayIt).toDouble();
                    }
                    i++;
                    if (i >= 3) {
                        break;
                    }
                }
                translate(translateParams[0], translateParams[1], translateParams[2]);
            }
            else if (transformObj.value("scale").isArray()) {
                QJsonArray scaleParamsArray = transformObj.value("scale").toArray();
                QJsonArray::iterator scaleParamsArrayIt = scaleParamsArray.begin();
                float scaleParams[] = {0.0, 0.0, 0.0};
                int i = 0;
                for (; scaleParamsArrayIt != scaleParamsArray.end(); ++scaleParamsArrayIt) {
                    if ((*scaleParamsArrayIt).isDouble()) {
                        scaleParams[i] = (*scaleParamsArrayIt).toDouble();
                    }
                    i++;
                    if (i >= 3) {
                        break;
                    }
                }
                scale(scaleParams[0], scaleParams[1], scaleParams[2]);
            }
        }
    }
}

// Loads the identity matrix.
glm::mat4 CModelViewMatrix::loadIdentity() {
    matrix = glm::mat4();
    setIdentityFlag();
    return matrix;
}

// Rotates.
glm::mat4 CModelViewMatrix::rotate(float angle, float x, float y, float z) {
    matrix = glm::rotate(matrix, (float)(angle * (M_PI/180.0)), glm::vec3(x, y, z));
    return matrix;
}

// Translates.
glm::mat4 CModelViewMatrix::translate(float x, float y, float z) {
    matrix = glm::translate(matrix, glm::vec3(x, y, z));
    return matrix;
}

// Scales.
glm::mat4 CModelViewMatrix::scale(float x, float y, float z) {
    matrix = glm::scale(matrix, glm::vec3(x, y, z));
}

bool CModelViewMatrix::identityFlag() {
    return bLoadIdentity;
}

void CModelViewMatrix::setIdentityFlag() {
    bLoadIdentity = true;
}

void CModelViewMatrix::clearIdentityFlag() {
    bLoadIdentity = false;
}
