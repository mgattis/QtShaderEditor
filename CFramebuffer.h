#ifndef CFRAMEBUFFER_H
#define CFRAMEBUFFER_H

#include "IProjectObject.h"

class CFramebuffer : public IProjectObject {
    bool bColorBuffer;
    bool bDepthBuffer;
    QString uniformName;
    // Something here to handle aspect ratios.

public:
    CFramebuffer();
    CFramebuffer(QString framebufferFile);
    CFramebuffer(QString framebufferFile, QJsonObject framebufferJson);
    ~CFramebuffer();

    bool initialize();

    void setViewPort(int iWidth, int iHeight);
};

#endif // CFRAMEBUFFER_H
