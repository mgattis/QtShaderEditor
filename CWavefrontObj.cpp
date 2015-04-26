#include "CWavefrontObj.h"

CWavefrontObj::CWavefrontObj()
{
    drawShader = NULL;

    wavefrontFile = "";
    materialPath = "";
}

CWavefrontObj::CWavefrontObj(QString wavefrontFile, QString MTLPath, CShader *drawShader)
{
    loadWavefront(wavefrontFile, MTLPath, drawShader);
}

CWavefrontObj::~CWavefrontObj()
{

}

// We are using Tiny Object Loader. We are going to sort mesh by material,
// not by "shape". Tiny Object Loader will convert all polys into triangles.
bool CWavefrontObj::loadWavefront(QString wavefrontFile, QString materialPath, CShader *drawShader) {
    if (materialPath.isEmpty()) {
        materialPath = "./";
    }
    if (!materialPath.endsWith('/')) {
        materialPath.append("/");
    }

    this->drawShader = drawShader;
    if (drawShader == NULL) {
        std::clog << "Draw shader needed!" << std::endl;
        return false;
    }

    std::string file = wavefrontFile.toStdString();
    std::string path = materialPath.toStdString();
    std::string error = tinyobj::LoadObj(tshapes, tmaterials, file.c_str(), path.c_str());

    if (error.size() == 0) {
        this->wavefrontFile = wavefrontFile;
        this->materialPath = materialPath;

        if (!generateBuffers()) {
            deleteModel();
            return false;
        }

        if (!loadBuffers()) {
            deleteModel();
            return false;
        }

        if (!loadMaterials()) {
            deleteModel();
            return false;
        }
    }
    else {
        std::clog << error << std::endl;
        return false;
    }

    return true;
}

bool CWavefrontObj::generateBuffers() {
    drawBuffers.clear();

    // Make sure we get at least one.
    // TODO: Test this.
    int numberOfMaterials = tmaterials.size() > 0 ? tmaterials.size() : 1;

    // One mesh per material.
    for (int i = 0; i < numberOfMaterials; i++) {
        SDrawBuffer dBuffer = {0};
        drawBuffers.push_back(dBuffer);
    }

    // Fill the draw buffers with data.
    for (int i = 0; i < tshapes.size(); i++) {
        // Not sure if 3 component faces are guaranteed by Tiny Object Loader.
        if (tshapes[i].mesh.indices.size() % 3 != 0) {
            return false;
        }
        for (int f = 0; f < tshapes[i].mesh.indices.size() / 3; f++) {
            // vertex/texCoord/normal
            int vID = tshapes[i].mesh.indices[3 * f + 0];
            int tID = tshapes[i].mesh.indices[3 * f + 1];
            int nID = tshapes[i].mesh.indices[3 * f + 2];
            int mat = tshapes[i].mesh.material_ids[f];

            // Get the vertex and add to appropriate buffer.
            int sizePosition = tshapes[i].mesh.positions.size();
            int sizeTexcoords = tshapes[i].mesh.texcoords.size();
            int sizeNormals = tshapes[i].mesh.normals.size();
            SBufferData vertexData;
            for (int j = 0; j < 3; j++) {
                if (3 * vID + j < sizePosition) {
                    vertexData.position[j] = tshapes[i].mesh.positions[3 * vID + j];
                }
                else {
                    vertexData.position[j] = 0.0;
                }

                if (3 * nID + j < sizeNormals) {
                    vertexData.normal[j] = tshapes[i].mesh.normals[3 * nID + j];
                }
                else {
                    vertexData.normal[j] = 0.0;
                }

                if (j < 2) {
                    if (3 * tID + j < sizeTexcoords) {
                        vertexData.texCoord[j] = tshapes[i].mesh.texcoords[2 * tID + j];
                    }
                    else {
                        vertexData.texCoord[j] = 0.0;
                    }
                }
            }
            drawBuffers[mat].bufferData.push_back(vertexData);
        }
    }

    return true;
}

bool CWavefrontObj::loadBuffers() {
    for (int i = 0; i < drawBuffers.size(); i++) {
        glGenVertexArrays(1, &drawBuffers[i].vao);
        glBindVertexArray(drawBuffers[i].vao);

        glGenBuffers(1, &drawBuffers[i].vbo);
        glBindBuffer(GL_ARRAY_BUFFER, drawBuffers[i].vbo);

        glBufferData(GL_ARRAY_BUFFER, drawBuffers[i].bufferData.size() * sizeof (SBufferData), drawBuffers[i].bufferData.data(), GL_STATIC_DRAW);

        // This is best called when assigning attributes.
        drawShader->useProgram(true);

        GLint positionAttrib = glGetAttribLocation(drawShader->getProgram(), "Position");
        if (positionAttrib != -1) {
            glEnableVertexAttribArray(positionAttrib);
            glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), 0);
        }

        GLint texcoordAttrib = glGetAttribLocation(drawShader->getProgram(), "Texcoord");
        if (texcoordAttrib != -1) {
            glEnableVertexAttribArray(texcoordAttrib);
            glVertexAttribPointer(texcoordAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), (void *)(3 * sizeof (GLfloat)));
        }

        GLint normalAttrib = glGetAttribLocation(drawShader->getProgram(), "Normal");
        if (normalAttrib != -1) {
            glEnableVertexAttribArray(normalAttrib);
            glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), (void *)(5 * sizeof (GLfloat)));
        }

        // Our buffers are now safe and secure with OpenGL.
        drawBuffers[i].bufferData.clear();
    }

    return true;
}

bool CWavefrontObj::loadMaterials() {
    if (tmaterials.size() == drawBuffers.size()) {
        for (int i = 0; i < drawBuffers.size(); i++) {
            glBindVertexArray(drawBuffers[i].vao);

            for (int j = 0; j < 3; j++) {
                drawBuffers[i].matAmbient[j] = tmaterials[i].ambient[j];
                drawBuffers[i].matDiffuse[j] = tmaterials[i].diffuse[j];
                drawBuffers[i].matSpecular[j] = tmaterials[i].specular[j];
                drawBuffers[i].matTransmittance[j] = tmaterials[i].transmittance[j];
                drawBuffers[i].matEmission[j] = tmaterials[i].emission[j];
            }

            drawBuffers[i].matShininess = tmaterials[i].shininess;
            drawBuffers[i].matIor = tmaterials[i].ior;
            drawBuffers[i].matDissolve = tmaterials[i].dissolve;
            drawBuffers[i].matIllum = tmaterials[i].illum;

            QString textureMaps[4];
            textureMaps[0] = tmaterials[i].ambient_texname.c_str();
            textureMaps[1] = tmaterials[i].diffuse_texname.c_str();
            textureMaps[2] = tmaterials[i].specular_texname.c_str();
            textureMaps[3] = tmaterials[i].normal_texname.c_str();

            GLuint *drawTextureMaps[4] = {&drawBuffers[i].ambientMap, &drawBuffers[i].diffuseMap, &drawBuffers[i].specularMap, &drawBuffers[i].normalMap};
            for (int j = 0; j < 4; j++) {
                if (textureMaps[j].size()) {
                    QImage *image = new QImage(materialPath + textureMaps[j]);
                    if (!image->isNull()) {
                        *image = image->convertToFormat(QImage::Format_RGBA8888);

                        // Make sure this does what I think it does.
                        glActiveTexture(GL_TEXTURE0 + j);

                        glGenTextures(1, drawTextureMaps[j]);
                        glBindTexture(GL_TEXTURE_2D, *drawTextureMaps[j]);

                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width(), image->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, ((const QImage *)image)->bits());

                        delete image;

                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

                        glGenerateMipmap(GL_TEXTURE_2D);
                    }
                }
            }
        }
    }

    return true;
}

void CWavefrontObj::deleteModel() {
    for (int i = 0; i < drawBuffers.size(); i++) {
        drawBuffers[i].bufferData.clear();

        glDeleteTextures(1, &drawBuffers[i].ambientMap);
        glDeleteTextures(1, &drawBuffers[i].diffuseMap);
        glDeleteTextures(1, &drawBuffers[i].specularMap);
        glDeleteTextures(1, &drawBuffers[i].normalMap);

        glDeleteBuffers(1, &drawBuffers[i].vbo);
        glDeleteVertexArrays(1, &drawBuffers[i].vao);
    }
}
