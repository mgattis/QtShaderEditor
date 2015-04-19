#ifndef CSHADER_H
#define CSHADER_H

#include "IProjectObject.h"

#include <GL/glew.h>
#include <QList>
#include <QString>
#include <QJsonArray>

class CShader : public IProjectObject {
private:
    QList<QString> fragmentSourceFiles;
    QList<QString> vertexSourceFiles;
    GLuint program;
    GLuint vs, fs;

public:
    CShader();
    CShader(QString shaderFile);
    CShader(QString shaderFile, QJsonObject shaderJson);
    ~CShader();

    bool initialize();

    void addVertexSourceFile(QString vertexFile);
    void addFragmentSourceFile(QString fragmentFile);
    bool build();
    bool useProgram();
    void deleteProgram();

    GLuint getProgram();
};

#endif // CSHADER_H