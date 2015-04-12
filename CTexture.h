#ifndef CTEXTURE_H
#define CTEXTURE_H

#include "IProjectObject.h"

class CTexture : public IProjectObject {
private:
    QString imageFile;
    QString internalFormat;
    QString channelType;
    QString uniformName;

public:
    CTexture();
    CTexture(QString textureFile);
    CTexture(QString textureFile, QJsonObject textureJson);
    ~CTexture();

    bool initialize();
};

#endif // CTEXTURE_H
