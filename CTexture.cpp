#include "CTexture.h"

CTexture::CTexture()
{
    imageFile = "";
    internalFormat = "auto";
    channelType = "auto";
    uniformName = "";
}

CTexture::CTexture(QString textureFile) : IProjectObject(textureFile) {
    imageFile = "";
    internalFormat = "auto";
    channelType = "auto";
    uniformName = "";
}

CTexture::CTexture(QString textureFile, QJsonObject textureJson) : IProjectObject(textureFile, textureJson) {
    imageFile = "";
    internalFormat = "auto";
    channelType = "auto";
    uniformName = "";
}

CTexture::~CTexture()
{

}

bool CTexture::initialize() {
    if (isJsonLoaded()) {
        if (userJson.value("imageFile").isString()) {
            imageFile = userJson.value("imageFile").toString();
        }
        if (userJson.value("textureParameters").isObject()) {
            QJsonObject textureParameters = userJson.value("textureParameters").toObject();
            if (textureParameters.value("internalFormat").isString()) {
                internalFormat = textureParameters.value("internalFormat").toString();
            }
            if (textureParameters.value("channelType").isString()) {
                channelType = textureParameters.value("internalFormat").toString();
            }
        }
        if (userJson.value("uniformName").isString()) {
            uniformName = userJson.value("uniformName").toString();
        }

        if (imageFile.size() == 0) {
            std::clog << "[WARNING]: texture/" << itemName.toStdString() << " does not specify a texture file." << std::endl;
        }
        if (uniformName.size() == 0) {
            std::clog << "[WARNING]: texture/" << itemName.toStdString() << " does not specify a uniform name." << std::endl;
        }

        this->bInitialized = true;
    }
    else {
        this->bInitialized = false;
    }

    // TODO: Add texture loading here.

    return this->bInitialized;
}
