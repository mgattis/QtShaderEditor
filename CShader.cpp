#include "CShader.h"

CShader::CShader()
{
    program = 0;
    vs = 0;
    fs = 0;
}

CShader::CShader(QString shaderFile) : IProjectObject(shaderFile) {
    program = 0;
    vs = 0;
    fs = 0;
}

CShader::CShader(QString shaderFile, QJsonObject shaderJson) : IProjectObject(shaderFile, shaderJson) {
    program = 0;
    vs = 0;
    fs = 0;
}

CShader::~CShader()
{
    deleteProgram();
}

bool CShader::initialize() {
    if (isJsonLoaded()) {
        if (userJson.value("vertexSourceFiles").isArray()) {
            QJsonArray vertexSourceFilesArray = userJson.value("vertexSourceFiles").toArray();

            QJsonArray::iterator vertexSourceFilesArrayIt = vertexSourceFilesArray.begin();
            for (; vertexSourceFilesArrayIt != vertexSourceFilesArray.end(); ++vertexSourceFilesArrayIt) {
                if ((*vertexSourceFilesArrayIt).isString()) {
                    QString vertexSource = (*vertexSourceFilesArrayIt).toString();
                    vertexSourceFiles.push_back(vertexSource);
                }
            }
        }

        if (userJson.value("fragmentSourceFiles").isArray()) {
            QJsonArray fragmentSourceFilesArray = userJson.value("vertexSourceFiles").toArray();

            QJsonArray::iterator fragmentSourceFilesArrayIt = fragmentSourceFilesArray.begin();
            for (; fragmentSourceFilesArrayIt != fragmentSourceFilesArray.end(); ++fragmentSourceFilesArrayIt) {
                if ((*fragmentSourceFilesArrayIt).isString()) {
                    QString fragmentSourceFile = (*fragmentSourceFilesArrayIt).toString();
                    vertexSourceFiles.push_back(fragmentSourceFile);
                }
            }
        }

        this->bInitialized = true;
    }
    else {
        this->bInitialized = false;
    }

    // TODO: Add shader loading and compilation here.

    return this->bInitialized;
}

void CShader::addVertexSourceFile(QString vertexSource) {

}

void CShader::addFragmentSourceFile(QString fragmentFile) {

}

bool CShader::build() {

}

bool CShader::link() {

}

bool CShader::useProgram() {

}

void CShader::deleteProgram() {
    // TODO: Make sure these are the right ones!
    if (program) {
        glDeleteProgram(program);
    }
    if (vs) {
        glDeleteShader(vs);
    }
    if (fs) {
        glDeleteShader(fs);
    }

    program = 0;
    vs = 0;
    fs = 0;
}

GLuint CShader::getProgram() {

}
