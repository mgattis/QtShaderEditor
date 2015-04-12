#include "CFramebuffer.h"

CFramebuffer::CFramebuffer() {
    bColorBuffer = true;
    bDepthBuffer = true;
    uniformName = "";
}

CFramebuffer::CFramebuffer(QString framebufferFile) : IProjectObject(framebufferFile) {
    bColorBuffer = true;
    bDepthBuffer = true;
    uniformName = "";
}

CFramebuffer::CFramebuffer(QString framebufferFile, QJsonObject framebufferJson) : IProjectObject(framebufferFile, framebufferJson) {
    bColorBuffer = true;
    bDepthBuffer = true;
    uniformName = "";
}

bool CFramebuffer::initialize() {
    if (isJsonLoaded()) {
        if (userJson.value("colorBuffer").isBool()) {
            bColorBuffer = userJson.value("colorBuffer").toBool();
        }
        if (userJson.value("depthBuffer").isBool()) {
            bDepthBuffer = userJson.value("depthBuffer").toBool();
        }

        if (userJson.value("dimensions").isObject()) {
            QJsonObject dimensions = userJson.value("dimensions").toObject();
            // TODO: Add aspect ratio.
        }

        if (userJson.value("uniformName").isString()) {
            uniformName = userJson.value("uniformName").toString();
        }

        this->bInitialized = true;
    }
    else {
        this->bInitialized = false;
    }

    return true;
}

void CFramebuffer::setViewPort(int iWidth, int iHeight) {
    viewPortWidth = iWidth;
    viewPortHeight = iHeight;

    // TODO: Add framebuffer resize...
}

CFramebuffer::~CFramebuffer()
{

}
