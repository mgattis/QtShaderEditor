#include "CModel.h"

CModel::CModel()
{
    objectFile = "";
    MTLPath = ".";
    drawShader = NULL;

    hasValidModel = false;
}

CModel::CModel(QString modelFile) : IProjectObject(modelFile) {
    objectFile = "";
    MTLPath = ".";
    drawShader = NULL;

    hasValidModel = false;
}

CModel::CModel(QString modelFile, QJsonObject modelJson) : IProjectObject(modelFile, modelJson) {
    objectFile = "";
	MTLPath = ".";
	drawShader = NULL;

    hasValidModel = false;
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

    // We need to call loadModel after all shaders have been built.c:

    return true;
}

void CModel::draw() {
    drawShader->useProgram(true);
    object.drawArrays();
}

bool CModel::loadModel() {
    if (objectFile.size() && MTLPath.size()) {
        objectFile = QDir(workingPath + objectFile).absolutePath();
        MTLPath = QDir(workingPath + MTLPath).absolutePath();
        bool bResult = object.loadWavefront(objectFile, QDir(MTLPath).absolutePath(), drawShader);

        if (bResult == false) {
            logWarning(QString("Unable to load model. '") + objectFile + QString("'."));
        }
        else {
            logInfo(QString("Model loaded. '") + objectFile + QString("'."));
        }

        hasValidModel = bResult;
        return bResult;
    }

    hasValidModel = false;
    return false;
}

CShader *CModel::getDrawShader() {
    return drawShader;
}

glm::mat4 CModel::getModelMatrix() {
    return modelMatrix.getMatrix();
}
