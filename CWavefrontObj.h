#ifndef CWAVEFRONTOBJ_H
#define CWAVEFRONTOBJ_H

#include "CShader.h"
#include "tinyobjloader/tiny_obj_loader.h"

#include <QString>
#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <QImage>
#include <QImageReader>

#pragma pack(0)

struct SBufferData {
    GLfloat position[3];
    GLfloat texCoord[2];
    GLfloat normal[3];
};

struct SDrawBuffer {
    GLuint vao;
    GLuint vbo;

    glm::vec3 matAmbient;
    glm::vec3 matDiffuse;
    glm::vec3 matSpecular;
    glm::vec3 matTransmittance;
    glm::vec3 matEmission;

    GLfloat matShininess;
    GLfloat matIor;
    GLfloat matDissolve;
    GLfloat matIllum;

    GLuint ambientMap;
    GLuint diffuseMap;
    GLuint specularMap;
    GLuint normalMap;

    std::vector<SBufferData> bufferData;
};

class CWavefrontObj {
private:
    std::vector<SDrawBuffer> drawBuffers;

    std::vector<tinyobj::shape_t> tshapes;
    std::vector<tinyobj::material_t> tmaterials;

    CShader *drawShader;

    QString wavefrontFile;
    QString materialPath;

private:
    bool generateBuffers();
    bool loadBuffers();
    bool loadMaterials();

    void setMaterialsForDraw(int iBuffer);

public:
    CWavefrontObj();
    // We are going to need the shader to load attributes.
    CWavefrontObj(QString wavefrontFile, QString materialPath, CShader *shader);
    ~CWavefrontObj();

    // Load our wavefront file.
    bool loadWavefront(QString wavefront, QString maaterialPath, CShader *drawShader);
    void drawArrays();
    void deleteModel();
};

#endif // CWAVEFRONTOBJ_H
