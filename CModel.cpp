#include "CModel.h"

CModel::CModel()
{
    objectFile = "";
    MTLPath = ".";
    drawShader = NULL;
}

CModel::CModel(QString modelFile) : IProjectObject(modelFile) {
    objectFile = "";
    MTLPath = ".";
    drawShader = NULL;
}

CModel::CModel(QString modelFile, QJsonObject modelJson) : IProjectObject(modelFile, modelJson) {
    objectFile = "";
	MTLPath = ".";
	drawShader = NULL;
}

CModel::~CModel()
{
    textureList.clear();
}

bool CModel::initialize() {
    if (isJsonLoaded()) {
        if (userJson.value("wavefront").isObject()) {
            QJsonObject wavefront = userJson.value("wavefront").toObject();
            if (wavefront.value("objectFile").isString()) {
                objectFile = wavefront.value("objectFile").toString();
            }
            if (wavefront.value("materialTemplateLibraryPath").isString()) {
                MTLPath = wavefront.value("materialTemplateLibraryPath").toString();
            }
        }

        if (userJson.value("textureList").isArray()) {
            QJsonArray textureListArray = userJson.value("textureList").toArray();

            QJsonArray::iterator textureListArrayIt = textureListArray.begin();
            for (; textureListArrayIt != textureListArray.end(); ++textureListArrayIt) {
                if ((*textureListArrayIt).isString()) {
                    QString texture = (*textureListArrayIt).toString();
                    if (lpProjectList->contains(texture)) {
                        IProjectObject *projectObject = (*lpProjectList)[texture];
                        if (projectObject->getItemType().compare("texture")) {
                            textureList.push_back((CTexture *)projectObject);
                        }
                    }
                }
            }
        }

        if (userJson.value("modelTransforms").isArray()) {
            QJsonArray modelTransformArray = userJson.value("modelTransforms").toArray();
            if (modelTransformArray.size()) {
                modelMatrix.userJsonTransform(modelTransformArray);
            }
        }

        if (userJson.value("drawShader").isString()) {
            QString drawShaderName = userJson.value("drawShader").toString();
            if (lpProjectList->contains(drawShaderName)) {
                IProjectObject *projectObject = (*lpProjectList)[drawShaderName];
                if (projectObject->getItemType().compare("shader") == 0) {
                    drawShader = (CShader *)projectObject;
                }
            }
        }
    }

    if (!drawShader) {
        return false;
    }

    return true;
}

void CModel::draw() {
    drawShader->useProgram(true);
    drawShader->uniformMat4("ModelMatrix", modelMatrix.getMatrix());

    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_TRIANGLE_STRIP);

    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);

    glEnd();
}

CShader *CModel::getDrawShader() {
    return drawShader;
}
