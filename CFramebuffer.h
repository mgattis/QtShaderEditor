#ifndef CFRAMEBUFFER_H
#define CFRAMEBUFFER_H

#include "IProjectObject.h"
#include "CAspectRatio.h"

class CFramebuffer : public IProjectObject {
    CAspectRatio aspect;

    bool bColorBuffer;
    bool bDepthBuffer;
    QString uniformName;
    // Something here to handle aspect ratios.

    GLuint framebufferObject;
    GLuint framebufferTexture;
    GLuint framebufferDepth;

    int iPhysicalBufferWidth;
    int iPhysicalBufferHeight;

public:
    CFramebuffer();
    CFramebuffer(QString framebufferFile);
    CFramebuffer(QString framebufferFile, QJsonObject framebufferJson);
    ~CFramebuffer();

    bool initialize();
    void setViewPort(int iWidth, int iHeight);

    int getBufferWidth();
    int getBufferHeight();

    bool makeBuffer(int iWidth, int iHeight);
    void deleteBuffer();
    bool useBuffer(bool bUse);
};

#endif // CFRAMEBUFFER_H
