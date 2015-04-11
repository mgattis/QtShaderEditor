#include "CStage.h"

CStage::CStage()
{
    bRunOnce = false;
}

CStage::CStage(QString stageFile) : IProjectObject(stageFile) {
    bRunOnce = false;
}

CStage::CStage(QString stageFile, QJsonObject stageJson) : IProjectObject(stageFile, stageJson) {
    bRunOnce = false;
}

bool CStage::initialize() {

    return true;
}

void CStage::run() {

}

void CStage::setViewPort(int iWidth, int iHeight) {

}

CStage::~CStage()
{

}
