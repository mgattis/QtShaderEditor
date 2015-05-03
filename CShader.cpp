#include "CShader.h"

#include <stdio.h>
#include <stdlib.h>

CShader::CShader()
{
    // Valid programs will never be 0.
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
            QJsonArray fragmentSourceFilesArray = userJson.value("fragmentSourceFiles").toArray();

            QJsonArray::iterator fragmentSourceFilesArrayIt = fragmentSourceFilesArray.begin();
            for (; fragmentSourceFilesArrayIt != fragmentSourceFilesArray.end(); ++fragmentSourceFilesArrayIt) {
                if ((*fragmentSourceFilesArrayIt).isString()) {
                    QString fragmentSourceFile = (*fragmentSourceFilesArrayIt).toString();
                    fragmentSourceFiles.push_back(fragmentSourceFile);
                }
            }
        }

        this->bInitialized = true;
    }
    else {
        this->bInitialized = false;
    }

    // Make sure we have at least one vertex shader.
    if (vertexSourceFiles.size() == 0) {
        logWarning("GLSL vertex source file(s) must be supplied for shader compilation.");
        this->bInitialized = false;
    }

    // Make sure we have at least one fragment shader.
    if (fragmentSourceFiles.size() == 0) {
        logWarning("GLSL fragment source file(s) must be supplied for shader compilation.");
        this->bInitialized = false;
    }

    // Make sure everything went all right and build.
    if (this->bInitialized) {
        this->bInitialized = build();
    }

    // Make sure build worked.
    if (this->bInitialized == false) {
        logWarning("Shader build failed. Ignoring shader.");
        // Clean up.
        deleteProgram();
    }

    // A little strange, but we are going to return true
    // because shaders can be ignored when drawing.
    this->bInitialized = true;
    return true;
}

void CShader::addVertexSourceFile(QString vertexFile) {
    vertexSourceFiles.push_back(vertexFile);
}

void CShader::addFragmentSourceFile(QString fragmentFile) {
    fragmentSourceFiles.push_back(fragmentFile);
}

// http://www.lighthouse3d.com/tutorials/glsl-tutorial/troubleshooting-the-infolog/
void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}

// http://www.lighthouse3d.com/tutorials/glsl-tutorial/troubleshooting-the-infolog/
void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}

bool CShader::build() {
    QString allVertexSource = "";
    QString allFragmentSource = "";
    char *buffer[1];
    QFile file;

	// Make shader objects. Two for now. Better add more in the future.
    // One for each source file.
    vs = glCreateShader(GL_VERTEX_SHADER);
    fs = glCreateShader(GL_FRAGMENT_SHADER);

	// Load all vertex files. Concatenate them into one string and give them to OGL.
    QList<QString>::iterator vertexSourceFilesIt = vertexSourceFiles.begin();
    for (; vertexSourceFilesIt != vertexSourceFiles.end(); ++vertexSourceFilesIt) {
        QString filePath = getWorkingPath().append(*vertexSourceFilesIt);
        file.setFileName(filePath);
        file.open(QIODevice::ReadOnly);

        if (file.isOpen()) {
            QString currentSource(file.readAll());

            allVertexSource.append(currentSource);
            allVertexSource.append("\n");
            file.close();
        }
        else {
            logWarning(QString("Could not open vertex shader for reading. '") + filePath + QString("'."));
        }
    }
    buffer[0] = (char *)calloc(allVertexSource.size() + 1, sizeof (char));
    QByteArray temp1 = allVertexSource.toLatin1();
    memcpy(buffer[0], temp1.data(), allVertexSource.size());
    glShaderSource(vs, 1, buffer, NULL);

    glCompileShader(vs);
    //printShaderInfoLog(vs);
    free(buffer[0]);

	// Load all fragment files. Concatenate them into one string and give them to OGL.
    QList<QString>::iterator fragmentSourceFilesIt = fragmentSourceFiles.begin();
    for (; fragmentSourceFilesIt != fragmentSourceFiles.end(); ++fragmentSourceFilesIt) {
        QString filePath = getWorkingPath().append(*fragmentSourceFilesIt);
        file.setFileName(filePath);
        file.open(QIODevice::ReadOnly);

        if (file.isOpen()) {
            QString currentSource(file.readAll());

            allFragmentSource.append(currentSource);
            allFragmentSource.append("\n");
            file.close();
        }
        else {
            logWarning(QString("Could not open fragment shader for reading. '") + filePath + QString("'."));
        }

    }
    buffer[0] = (char *)calloc(allFragmentSource.size() + 1, sizeof (char));
    QByteArray temp2 = allFragmentSource.toLatin1();
    memcpy(buffer[0], temp2.data(), allFragmentSource.size());
    glShaderSource(fs, 1, buffer, NULL);

    glCompileShader(fs);
    //printShaderInfoLog(fs);
    free(buffer[0]);

    // Link the program.
    program = glCreateProgram();

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    // Make the shader program.
    glLinkProgram(program);
    printProgramInfoLog(program);

    // Make sure everything worked. Again.
    GLint success = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    // If the build failed, return false.
    if (success == GL_FALSE) {
        printProgramInfoLog(program);
        return false;
    }

    glBindFragDataLocation(program, 0, "ColorOut");
    glBindFragDataLocation(program, 1, "DepthOut");

    return true;
}

// Does not build it for you.
bool CShader::useProgram(bool bUse) {
    if (bUse) {
        if (program) {
            glUseProgram(program);
            return true;
        }
        else {
            return false;
        }
    }
    else {
        glUseProgram(0);
        return true;
    }
}

void CShader::deleteProgram() {
    // We should not need to make sure they "exist."
    glDeleteProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    program = 0;
    vs = 0;
    fs = 0;
}

void CShader::uniform1f(QString uniform, float x) {
    // Our shader program needs to be active.
    if (program) {
        if (useProgram(true)) {
            GLint uniformLocation = glGetUniformLocation(program, uniform.toStdString().c_str());
            glUniform1f(uniformLocation, x);
        }
    }
}

void CShader::uniform2f(QString uniform, float x, float y) {
    // Our shader program needs to be active.
    if (program) {
        if (useProgram(true)) {
            GLint uniformLocation = glGetUniformLocation(program, uniform.toStdString().c_str());
            glUniform2f(uniformLocation, x, y);
        }
    }
}

void CShader::uniform2f(QString uniform, glm::vec2 v1) {
    if (program) {
        if (useProgram(true)) {
            GLint uniformLocation = glGetUniformLocation(program, uniform.toStdString().c_str());
            glUniform2fv(uniformLocation, 1, glm::value_ptr(v1));
        }
    }
}

void CShader::uniform3f(QString uniform, float x, float y, float z) {
    // Our shader program needs to be active.
    if (program) {
        if (useProgram(true)) {
            GLint uniformLocation = glGetUniformLocation(program, uniform.toStdString().c_str());
            glUniform3f(uniformLocation, x, y, z);
        }
    }
}

void CShader::uniform3f(QString uniform, glm::vec3 v1) {
    if (program) {
        if (useProgram(true)) {
            GLint uniformLocation = glGetUniformLocation(program, uniform.toStdString().c_str());
            glUniform3fv(uniformLocation, 1, glm::value_ptr(v1));
        }
    }
}


void CShader::uniform4f(QString uniform, float x, float y, float z, float w) {
    // Our shader program needs to be active.
    if (program) {
        if (useProgram(true)) {
            GLint uniformLocation = glGetUniformLocation(program, uniform.toStdString().c_str());
            glUniform4f(uniformLocation, x, y, z, w);
        }
    }
}

void CShader::uniform4f(QString uniform, glm::vec4 v1) {
    if (program) {
        if (useProgram(true)) {
            GLint uniformLocation = glGetUniformLocation(program, uniform.toStdString().c_str());
            glUniform4fv(uniformLocation, 1, glm::value_ptr(v1));
        }
    }
}


void CShader::uniformMat4(QString uniform, glm::mat4 mat) {
    // Our shader program needs to be active.
    if (program) {
        if (useProgram(true)) {
            QByteArray temp = uniform.toLatin1();
            GLuint uniformLocation = glGetUniformLocation(program, temp.data());
            glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat));
        }
    }
}

void CShader::uniformMat3(QString uniform, glm::mat3 mat) {
    if (program) {
        if (useProgram(true)) {
            QByteArray temp = uniform.toLatin1();
            GLuint uniformLocation = glGetUniformLocation(program, temp.data());
            glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat));
        }
    }
}

void CShader::uniform1ui(QString uniform, GLuint index) {
    // Our shader program needs to be active.
    if (program) {
        if (useProgram(true)) {
            GLint uniformLocation = glGetUniformLocation(program, uniform.toStdString().c_str());
            glUniform1i(uniformLocation, index);
        }
    }
}

GLuint CShader::getProgram() {
    // Zero basically means it does not exist.
    return program;
}

// Updates in the shader whether you like it or not.
// Given to us by the project object.
void CShader::setRunTime(float fRunTime) {
    // Load in time to our shader.
    // Even if it does not exist, we will be fine.
    uniform1f("Time", fRunTime);
}
