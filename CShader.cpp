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

    build();
    return this->bInitialized;
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

    vs = glCreateShader(GL_VERTEX_SHADER);
    fs = glCreateShader(GL_FRAGMENT_SHADER);

    QList<QString>::iterator vertexSourceFilesIt = vertexSourceFiles.begin();
    for (; vertexSourceFilesIt != vertexSourceFiles.end(); ++vertexSourceFilesIt) {
        file.setFileName(*vertexSourceFilesIt);
        file.open(QIODevice::ReadOnly);

        if (file.isOpen()) {
            QString currentSource(file.readAll());

            allVertexSource.append(currentSource);
            allVertexSource.append("\n");
            file.close();
        }
    }
    buffer[0] = (char *)allVertexSource.toStdString().c_str();
    glShaderSource(vs, 1, buffer, NULL);

    QList<QString>::iterator fragmentSourceFilesIt = fragmentSourceFiles.begin();
    for (; fragmentSourceFilesIt != fragmentSourceFiles.end(); ++fragmentSourceFilesIt) {
        file.setFileName(*fragmentSourceFilesIt);
        file.open(QIODevice::ReadOnly);

        if (file.isOpen()) {
            QString currentSource(file.readAll());

            allFragmentSource.append(currentSource);
            allFragmentSource.append("\n");
            file.close();
        }
    }

    buffer[0] = (char *)allFragmentSource.toStdString().c_str();
    glShaderSource(fs, 1, buffer, NULL);

    glCompileShader(vs);
    printShaderInfoLog(vs);
    glCompileShader(fs);
    printShaderInfoLog(fs);

    program = glCreateProgram();

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    printProgramInfoLog(program);

    return true;
}

bool CShader::useProgram() {
    if (!program) {
        build();
    }

    if (program) {
        glUseProgram(program);
    }
}

void CShader::deleteProgram() {
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
    return program;
}
