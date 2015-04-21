#include "CFramebuffer.h"

CFramebuffer::CFramebuffer() {
    bColorBuffer = true;
    bDepthBuffer = true;
    uniformName = "";

    framebufferObject = 0;
    framebufferTexture = 0;
    framebufferDepth = 0;

    iPhysicalBufferWidth = 0;
    iPhysicalBufferHeight = 0;
}

CFramebuffer::CFramebuffer(QString framebufferFile) : IProjectObject(framebufferFile) {
    bColorBuffer = true;
    bDepthBuffer = true;
    uniformName = "";

    framebufferObject = 0;
    framebufferTexture = 0;
    framebufferDepth = 0;

    iPhysicalBufferWidth = 0;
    iPhysicalBufferHeight = 0;
}

CFramebuffer::CFramebuffer(QString framebufferFile, QJsonObject framebufferJson) : IProjectObject(framebufferFile, framebufferJson) {
    bColorBuffer = true;
    bDepthBuffer = true;
    uniformName = "";

    framebufferObject = 0;
    framebufferTexture = 0;
    framebufferDepth = 0;

    iPhysicalBufferWidth = 0;
    iPhysicalBufferHeight = 0;
}

CFramebuffer::~CFramebuffer()
{
    deleteBuffer();
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
            if (dimensions.value("pixelWidth").isDouble()) {
                int pixelWidth = dimensions.value("pixelWidth").toDouble();
                aspect.setPixelWidth(pixelWidth);
            }
            if (dimensions.value("pixelHeight").isDouble()) {
                int pixelHeight = dimensions.value("pixelHeight").toDouble();
                aspect.setPixelHeight(pixelHeight);
            }
            if (dimensions.value("windowWidth").isDouble()) {
                aspect.setWindowWidth(dimensions.value("windowWidth").toDouble());
            }
            if (dimensions.value("windowHeight").isDouble()) {
                aspect.setWindowHeight(dimensions.value("windowHeight").toDouble());
            }
            if (dimensions.value("multiplier").isDouble()) {
                aspect.setMultiplier(dimensions.value("multiplier").toDouble());
            }
        }

        if (userJson.value("uniformName").isString()) {
            uniformName = userJson.value("uniformName").toString();
        }

        this->bInitialized = true;
    }
    else {
        this->bInitialized = false;
    }

    // Force these to be true regardless of user settings. For now.
    bColorBuffer = true;
    bDepthBuffer = true;

    return true;
}

// Create and configure our buffer. This will be taken care of by setViewPort.
bool CFramebuffer::makeBuffer(int iWidth, int iHeight) {
    iPhysicalBufferWidth = iWidth;
    iPhysicalBufferHeight = iHeight;

    glGenFramebuffers(1, &framebufferObject);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferObject);

    glGenTextures(1, &framebufferTexture);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iPhysicalBufferWidth, iPhysicalBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glGenRenderbuffers(1, &framebufferDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, framebufferDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, iPhysicalBufferWidth, iPhysicalBufferHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, framebufferDepth);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, framebufferTexture, 0);
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);

    // If any of that above failed, clean up and report failure.
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        iPhysicalBufferWidth = 0;
        iPhysicalBufferWidth = 0;

        deleteBuffer();
        return false;
    }

    // We will handle all of this in CStage.
    //useBuffer(true);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return true;
}

// Delete everything.
void CFramebuffer::deleteBuffer() {
    glDeleteFramebuffers(1, &framebufferObject);
    glDeleteTextures(1, &framebufferTexture);
    glDeleteRenderbuffers(1, &framebufferDepth);
    framebufferObject = 0;
    framebufferTexture = 0;
    framebufferDepth = 0;
}

bool CFramebuffer::useBuffer(bool bUse) {
    if (framebufferObject) {
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferObject);
        glViewport(0, 0, iPhysicalBufferWidth, iPhysicalBufferHeight);
        return true;
    }
    return false;
}

void CFramebuffer::setViewPort(int iWidth, int iHeight) {
    iViewPortWidth = iWidth;
    iViewPortHeight = iHeight;

    aspect.getAspectRatio(iWidth, iHeight);
    iWidth = aspect.getViewWidth();
    iHeight = aspect.getViewHeight();

    if (iWidth != iPhysicalBufferWidth || iHeight != iPhysicalBufferHeight) {
        deleteBuffer();

        if (this->bInitialized) {
            makeBuffer(iWidth, iHeight);
        }
    }
}
