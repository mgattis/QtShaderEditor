#ifndef CSTAGE_H
#define CSTAGE_H

#include "IProjectObject.h"

#include <QList>

class CStage : public IProjectObject {
private:
    QList<IProjectObject *> modelList;
    QList<IProjectObject *> inputFramebuffers;
    IProjectObject *outputFramebuffer;
    bool bRunOnce;

public:
    CStage();
    CStage(QString stageFile);
    CStage(QString stageFile, QJsonObject stageJson);
    ~CStage();

    bool initialize();
    void run();

    void setViewPort(int iWidth, int iHeight);
};

#endif // CSTAGE_H
