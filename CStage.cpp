#include "CStage.h"

CStage::CStage()
{
    projectionMatrix = NULL;
    outputFramebuffer = NULL;

    bRunOnce = false;
    bRanOnce = false;
}

CStage::CStage(QString stageFile) : IProjectObject(stageFile) {
    projectionMatrix = NULL;
    outputFramebuffer = NULL;

    bRunOnce = false;
    bRanOnce = false;

}

CStage::CStage(QString stageFile, QJsonObject stageJson) : IProjectObject(stageFile, stageJson) {
    projectionMatrix = NULL;
    outputFramebuffer = NULL;

    bRunOnce = false;
    bRanOnce = false;
}

CStage::~CStage()
{
    if (projectionMatrix) {
        delete projectionMatrix;
    }
}

bool CStage::initialize() {
    if (bJsonLoaded) {
        if (userJson.value("generateProjectionMatrixOrtho").isObject()) {
            COrtho *orthoMatrix = new COrtho();
            orthoMatrix->userJsonProjection(userJson.value("generateProjectionMatrixOrtho").toObject());
            projectionMatrix = (CProjectionMatrix *)orthoMatrix;
        }
        else if (userJson.value("generateProjectionMatrixPerspective").isObject()) {
            CPerspective *perspectiveMatrix = new CPerspective();
            perspectiveMatrix->userJsonProjection(userJson.value("generateProjectionMatrixPerspective").toObject());
            projectionMatrix = (CProjectionMatrix *)perspectiveMatrix;
        }
        else {
            COrtho *orthoMatrix = new COrtho();
            orthoMatrix->setLeft(-1.0);
            orthoMatrix->setRight(1.0);
            orthoMatrix->setBottom(-1.0);
            orthoMatrix->setTop(1.0);
            orthoMatrix->setZNear(-1.0);
            orthoMatrix->setZFar(1.0);
            projectionMatrix = (CProjectionMatrix *)orthoMatrix;
        }

        if (userJson.value("viewTransforms").isArray()) {
            QJsonArray viewTransformsArray = userJson.value("viewTransforms").toArray();
            viewMatrix.userJsonTransform(viewTransformsArray);
        }

        if (userJson.value("inputFramebufferList").isArray()) {
            QJsonArray inputFramebufferListArray = userJson.value("inputFramebufferList").toArray();
            QJsonArray::iterator inputFramebufferListArrayIt = inputFramebufferListArray.begin();
            for (;inputFramebufferListArrayIt != inputFramebufferListArray.end(); ++inputFramebufferListArrayIt) {
                if ((*inputFramebufferListArrayIt).isString()) {
                    QString framebufferName = (*inputFramebufferListArrayIt).toString();
                    if ((*lpProjectList).contains(framebufferName)) {
                        IProjectObject *projectObject = (*lpProjectList)[framebufferName];
                        if (projectObject->getItemType().compare("framebuffer") == 0) {
                            inputFramebuffersList.push_back((CFramebuffer *)projectObject);
                        }
                    }
                }
            }
        }

        if (userJson.value("outputFramebuffer").isString()) {
            QString framebufferName = userJson.value("outputFramebuffer").toString("context");
            if (framebufferName.compare("context") != 0) {
                if ((*lpProjectList).contains(framebufferName)) {
                    IProjectObject *projectObject = (*lpProjectList)[framebufferName];
                    if (projectObject->getItemType().compare("framebuffer") == 0) {
                        outputFramebuffer = (CFramebuffer *)projectObject;
                    }
                }
            }
        }

        if (userJson.value("modelList").isArray()) {
            QJsonArray modelListArray = userJson.value("modelList").toArray();
            QJsonArray::iterator modelListArrayIt = modelListArray.begin();
            for (; modelListArrayIt != modelListArray.end(); ++modelListArrayIt) {
                if ((*modelListArrayIt).isString()) {
                    QString modelName = (*modelListArrayIt).toString();
                    if ((*lpProjectList).contains(modelName)) {
                        IProjectObject *projectObject = (*lpProjectList)[modelName];
                        if (projectObject->getItemType().compare("model") == 0) {
                            modelList.push_back((CModel *)projectObject);
                        }
                    }
                }
            }
        }

        if (userJson.value("runOnce").isBool()) {
            bRunOnce = userJson.value("runOnce").toBool();
        }
    }

    if (modelList.size() == 0) {
        logWarning(QString("No models in stage. '") + getFullIdentifier() + QString("'."));
    }

    return true;
}

// Executes all of the stages.
void CStage::run() {
    int iBufferWidth = iViewPortWidth;
    int iBufferHeight = iViewPortHeight;
    GLuint framebuffer = 0;

    // Do we need to run once again?
    if (outputFramebuffer) {
        if (outputFramebuffer->resizedFlag()) {
            bRanOnce = false;
            outputFramebuffer->clearResizedFlag();
        }
    }
    else {
        bRunOnce = false;
    }

    // Has it ran? Has it not ran? Does it need to run again?
    // How many licks does it take to get to the center of a
    // Tootsie Pop?
    if ((bRunOnce && !bRanOnce) || !bRunOnce) {
        // Grab our output framebuffer info and enable it.
        if (outputFramebuffer) {
            iBufferWidth = outputFramebuffer->getBufferWidth();
            iBufferHeight = outputFramebuffer->getBufferHeight();
            outputFramebuffer->useBuffer(true);
        }
        else {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, iViewPortWidth, iViewPortHeight);
        }

        // Get our projection matrix sorted out.
        projectionMatrix->getProjectionMatrix(iBufferWidth, iBufferHeight);

        // Get our view matrix sorted out.
        glm::mat4 viewMatrix = camera->getMatrixFromPosition();
        if (this->viewMatrix.identityFlag()) {
           // Only user transforms.
           viewMatrix = this->viewMatrix.getMatrix();
        }
        else {
           // Apply user transforms TO the camera.
           viewMatrix = this->viewMatrix.getMatrix() * viewMatrix;
        }

        // About to start drawing. Ready the buffer.
        glClearColor(1.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glEnable( GL_BLEND );
        glEnable( GL_COLOR_MATERIAL );
        glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
        glBlendEquation( GL_FUNC_ADD );
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

        // Cycle through our models.
        QList<CModel *>::iterator modelListIt = modelList.begin();
        for (; modelListIt != modelList.end(); ++modelListIt) {
            CModel *model = (*modelListIt);
            CShader *drawShader = model->getDrawShader();

            // Give our matricies to the shader.
            drawShader->uniformMat4("ViewMatrix", viewMatrix);
            glm::mat4 projectionMatrix = this->projectionMatrix->getProjectionMatrix(iBufferWidth, iBufferHeight);
            drawShader->uniformMat4("ProjectionMatrix", projectionMatrix);
            glm::mat4 modelMatrix = model->getModelMatrix();
            drawShader->uniformMat4("ModelMatrix", modelMatrix);
            glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(viewMatrix * modelMatrix));
            drawShader->uniformMat3("NormalMatrix", normalMatrix);

            // Give our input framebuffers to the shader.
            QList<CFramebuffer *>::iterator framebufferListIt = inputFramebuffersList.begin();
            for (; framebufferListIt != inputFramebuffersList.end(); ++framebufferListIt) {
                CFramebuffer *framebuffer = (*framebufferListIt);
                QString framebufferName = framebuffer->getUniformName();
                GLuint framebufferID = framebuffer->getFrameBufferTexture();
                drawShader->uniform1ui(framebufferName, framebufferID);
            }

            model->draw();
        }

        bRanOnce = true;
    }
}
