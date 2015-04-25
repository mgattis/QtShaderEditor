#ifndef CSTAGE_H
#define CSTAGE_H

#include "IProjectObject.h"
#include "CProjectionMatrix.h"
#include "COrtho.h"
#include "CPerspective.h"
#include "CModelViewMatrix.h"
#include "CFramebuffer.h"
#include "CModel.h"

#include <QList>

class CStage : public IProjectObject {
private:
    QList<CModel *> modelList;
    QList<CFramebuffer *> inputFramebuffersList;
    CFramebuffer *outputFramebuffer;

    CProjectionMatrix *projectionMatrix;
    CModelViewMatrix viewMatrix;

    bool bRunOnce;
    bool bRanOnce;

public:
    CStage();
    CStage(QString stageFile);
    CStage(QString stageFile, QJsonObject stageJson);
    virtual ~CStage();

    bool initialize();
    void run();
};

#endif // CSTAGE_H
