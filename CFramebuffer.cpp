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
