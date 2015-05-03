#ifndef CMODEL_H
#define CMODEL_H

#include "IProjectObject.h"
#include "CTexture.h"
#include "CShader.h"
#include "CModelViewMatrix.h"
#include "CWavefrontObj.h"

#include <iostream>
#include <string>
#include <QJsonArray>

class CModel : public IProjectObject {
private:
    QString objectFile;
    QString MTLPath;

    QList<CTexture *> textureList;
    CShader *drawShader;

    CModelViewMatrix modelMatrix;

    bool bHasValidModel;
    CWavefrontObj object;

public:
    CModel();
    CModel(QString modelFile);
    CModel(QString modelFile, QJsonObject modelJson);
    ~CModel();

    bool initialize();

    bool loadModel();
    void draw();
    bool hasValidModel();
    // We kinda need this bound for framebuffer stuff.
    void bindVAO();

    CShader *getDrawShader();
    glm::mat4 getModelMatrix();
};

#endif // CMODEL_H
