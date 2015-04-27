#ifndef CSHADER_H
#define CSHADER_H

#include "IProjectObject.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

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
    bool useProgram(bool bUse);
    void deleteProgram();

    void uniform1f(QString uniform, float x);
    void uniform2f(QString uniform, float x, float y);
    void uniform2f(QString uniform, glm::vec2 v1);
    void uniform3f(QString uniform, float x, float y, float z);
    void uniform3f(QString uniform, glm::vec3 v1);
    void uniform4f(QString uniform, float x, float y, float z, float w);
    void uniform4f(QString uniform, glm::vec4 v1);
    void uniform1ui(QString uniform, GLuint index);
    void uniformMat4(QString uniform, glm::mat4 mat);
    void uniformMat3(QString uniform1f, glm::mat3 mat);

    GLuint getProgram();

    // Given to us by the project object.
    void setRunTime(float fRunTime);
};

#endif // CSHADER_H
