#ifndef CMODEL_H
#define CMODEL_H

#include "IProjectObject.h"
#include "CTexture.h"
#include "CShader.h"

#include <QJsonArray>

class CModel : public IProjectObject {
private:
    QString objectFile;
    QString MTLPath;

    QList<CTexture *> textureList;
    CShader *drawShader;
public:
    CModel();
    CModel(QString modelFile);
    CModel(QString modelFile, QJsonObject modelJson);
    ~CModel();

    bool initialize();
    void draw();
};

#endif // CMODEL_H
