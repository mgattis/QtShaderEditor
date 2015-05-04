#include "CFramebuffer.h"

CFramebuffer::CFramebuffer() {
    bColorBuffer = true;
    bDepthBuffer = true;
    uniformName = "";

    framebufferObject = 0;
    framebufferTexture = 0;
    framebufferDepth = 0;

    iPhysicalBufferWidth = 256;
    iPhysicalBufferHeight = 256;

    bResized = false;
}

CFramebuffer::CFramebuffer(QString framebufferFile) : IProjectObject(framebufferFile) {
    bColorBuffer = true;
    bDepthBuffer = true;
    uniformName = "";

    framebufferObject = 0;
    framebufferTexture = 0;
    framebufferDepth = 0;

    iPhysicalBufferWidth = 256;
    iPhysicalBufferHeight = 256;

    bResized = false;
}

CFramebuffer::CFramebuffer(QString framebufferFile, QJsonObject framebufferJson) : IProjectObject(framebufferFile, framebufferJson) {
    bColorBuffer = true;
    bDepthBuffer = true;
    uniformName = "";

    framebufferObject = 0;
    framebufferTexture = 0;
    framebufferDepth = 0;

    iPhysicalBufferWidth = 256;
    iPhysicalBufferHeight = 256;

    bResized = false;
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

    glGenTextures(1, &framebufferTexture);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glGenTextures(1, &framebufferDepth);
    glBindTexture(GL_TEXTURE_2D, framebufferDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, iWidth, iHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glGenFramebuffers(1, &framebufferObject);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferObject);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, framebufferTexture, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, framebufferDepth, 0);

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
    glDeleteTextures(1, &framebufferDepth);
    glDeleteTextures(1, &framebufferTexture);
    framebufferObject = 0;
    framebufferTexture = 0;
    framebufferDepth = 0;
}

bool CFramebuffer::useBuffer(bool bUse) {
	if (framebufferObject && bUse) {
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferObject);

        GLuint attachments[] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};
        glDrawBuffers(2, attachments);

        glViewport(0, 0, iPhysicalBufferWidth, iPhysicalBufferHeight);

        return true;
    }
    else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    return false;
}

int CFramebuffer::getBufferWidth() {
    return this->iPhysicalBufferWidth;
}

int CFramebuffer::getBufferHeight() {
    return this->iPhysicalBufferHeight;
}

QString CFramebuffer::getUniformName() {
    return this->uniformName;
}

GLuint CFramebuffer::getFrameBufferObject() {
    return this->framebufferObject;
}

GLuint CFramebuffer::getFrameBufferTexture() {
    return this->framebufferTexture;
}

GLuint CFramebuffer::getFrameBufferDepth() {
    return this->framebufferDepth;
}

bool CFramebuffer::resizedFlag() {
    return this->bResized;
}

void CFramebuffer::setResizedFlag() {
    bResized = true;
}

void CFramebuffer::clearResizedFlag() {
    bResized = false;
}

void CFramebuffer::setViewPort(int iWidth, int iHeight) {
    iViewPortWidth = iWidth;
    iViewPortHeight = iHeight;

    // Make a new framebuffer if the window size haas changed.
    aspect.getAspectRatio(iWidth, iHeight);
    iWidth = aspect.getViewWidth();
    iHeight = aspect.getViewHeight();

    if (iWidth != iPhysicalBufferWidth || iHeight != iPhysicalBufferHeight) {
        deleteBuffer();

        if (this->bInitialized) {
            makeBuffer(iWidth, iHeight);
            setResizedFlag();
        }
    }
}
