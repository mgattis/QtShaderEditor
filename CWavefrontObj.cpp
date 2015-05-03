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

void CWavefrontObj::_genNormals(std::vector<SBufferData> *triangle) {
    glm::vec3 U = glm::vec3(triangle->at(1).x - triangle->at(0).x, triangle->at(1).y - triangle->at(0).y, triangle->at(1).z - triangle->at(0).z);
    glm::vec3 V = glm::vec3(triangle->at(2).x - triangle->at(0).x, triangle->at(2).y - triangle->at(0).y, triangle->at(2).z - triangle->at(0).z);
    glm::vec3 normal = glm::normalize(glm::cross(U, V));
    for (int i = 0; i < 3; i++) {
        triangle->at(i).nx = normal.x;
        triangle->at(i).ny = normal.y;
        triangle->at(i).nz = normal.z;
    }
}

// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
void CWavefrontObj::_genTangents(std::vector<SBufferData> *triangle) {
    glm::vec3 v0 = glm::vec3(triangle->at(0).x, triangle->at(0).y, triangle->at(0).z);
    glm::vec3 v1 = glm::vec3(triangle->at(1).x, triangle->at(1).y, triangle->at(1).z);
    glm::vec3 v2 = glm::vec3(triangle->at(2).x, triangle->at(2).y, triangle->at(2).z);

    glm::vec2 uv0 = glm::vec2(triangle->at(0).x, triangle->at(0).y);
    glm::vec2 uv1 = glm::vec2(triangle->at(1).x, triangle->at(1).y);
    glm::vec2 uv2 = glm::vec2(triangle->at(2).x, triangle->at(2).y);

    glm::vec3 deltaPos1 = v1 - v0;
    glm::vec3 deltaPos2 = v2 - v0;

    glm::vec2 deltaUV1 = uv1 - uv0;
    glm::vec2 deltaUV2 = uv2 - uv0;

    float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
    glm::vec3 tangent = (deltaPos1 * deltaUV2.y  - deltaPos2 * deltaUV1.y)*r;
    glm::vec3 bitangent = (deltaPos2 * deltaUV1.x  - deltaPos1 * deltaUV2.x)*r;

    for (int i = 0; i < 3; i++) {
        glm::vec3 n = glm::vec3(triangle->at(i).nx, triangle->at(i).ny, triangle->at(i).nz);
        glm::vec3 tangentF = glm::normalize(tangent - n * glm::dot(n, tangent));

        float fWind = 1.0;
        if (glm::dot(glm::cross(n, tangentF), bitangent) < 0.0f) {
            fWind = -1.0;
        }

        triangle->at(i).Tx = tangentF.x;
        triangle->at(i).Ty = tangentF.y;
        triangle->at(i).Tz = tangentF.z;
        triangle->at(i).Tw = fWind;
    }
}

bool CWavefrontObj::generateBuffers() {
    drawBuffers.clear();

    // Make sure we get at least one.
    // TODO: Test this.
    int numberOfMaterials = tmaterials.size() > 0 ? tmaterials.size() : 1;

    // One mesh per material.
    for (int i = 0; i < numberOfMaterials; i++) {
        SDrawBuffer drawBuffer;
        drawBuffers.push_back(drawBuffer);
    }

    // Fill the draw buffers with data.
    for (int shapeIt = 0; shapeIt < tshapes.size(); shapeIt++) {
        // It is possible that our indices may be missing some values.
        // We need to check and make sure they are there!
        // We will exploit the fact that if one of the following are zero,
        // that attribute is missing.
        int sizePosition = tshapes[shapeIt].mesh.positions.size();
        int sizeTexcoords = tshapes[shapeIt].mesh.texcoords.size();
        int sizeNormals = tshapes[shapeIt].mesh.normals.size();
        int actualAttribsPerIndex = 0;

        // Iterate through all triangles.
        int totalTriangles = tshapes[shapeIt].mesh.indices.size() / 3;
        for (int face = 0; face < totalTriangles; face++) {
            int mat = tshapes[shapeIt].mesh.material_ids[face];
            int matCount = tshapes[shapeIt].mesh.material_ids.size();

            std::vector<SBufferData> triangle;
            bool bNeedsNormal = false;

            // Iterate through vertex
            for (int index = 0; index < 3; index++) {
                int lookup = tshapes[shapeIt].mesh.indices[3 * face + index];
                SBufferData vertexData;

                if (3 * lookup + 2 < sizePosition) {
                    vertexData.x = tshapes[shapeIt].mesh.positions[3 * lookup + 0];
                    vertexData.y = tshapes[shapeIt].mesh.positions[3 * lookup + 1];
                    vertexData.z = tshapes[shapeIt].mesh.positions[3 * lookup + 2];
                }
                else {
                    vertexData.x = 0.0;
                    vertexData.y = 0.0;
                    vertexData.z = 0.0;
                }
                if (2 * lookup + 1 < sizeTexcoords) {
                    vertexData.u = tshapes[shapeIt].mesh.texcoords[2 * lookup + 0];
                    vertexData.v = tshapes[shapeIt].mesh.texcoords[2 * lookup + 1];
                }
                else {
                    vertexData.u = 0.0;
                    vertexData.v = 0.0;
                }
                if (3 * lookup + 2 < sizeNormals) {
                    vertexData.nx = tshapes[shapeIt].mesh.normals[3 * lookup + 0];
                    vertexData.ny = tshapes[shapeIt].mesh.normals[3 * lookup + 1];
                    vertexData.nz = tshapes[shapeIt].mesh.normals[3 * lookup + 2];
                }
                else {
                    vertexData.nx = 0.0;
                    vertexData.ny = 0.0;
                    vertexData.nz = 0.0;
                    bNeedsNormal = true;
                }

                triangle.push_back(vertexData);
            }

            if (bNeedsNormal) {
                _genNormals(&triangle);
            }

            _genTangents(&triangle);

            for (int i = 0; i < 3; i++) {
                drawBuffers[mat].bufferData.push_back(triangle[i]);
            }

            triangle.clear();
        }
    }

    for (int i = 0; i < drawBuffers.size(); i++) {
        drawBuffers[i].count = drawBuffers[i].bufferData.size();
    }

    return true;
}

bool CWavefrontObj::loadBuffers() {
    for (int i = 0; i < drawBuffers.size(); i++) {
        glGenVertexArrays(1, &drawBuffers[i].vao);
        glBindVertexArray(drawBuffers[i].vao);

        glGenBuffers(1, &drawBuffers[i].vbo);
        glBindBuffer(GL_ARRAY_BUFFER, drawBuffers[i].vbo);

        glBufferData(GL_ARRAY_BUFFER, 12 * drawBuffers[i].count * sizeof (GLfloat), &(drawBuffers[i].bufferData[0]), GL_STATIC_DRAW);

        // This is best called when assigning attributes.
        drawShader->useProgram(true);

        GLint positionAttrib = glGetAttribLocation(drawShader->getProgram(), "Position");
        if (positionAttrib != -1) {
            glEnableVertexAttribArray(positionAttrib);
            glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 12 * sizeof (GLfloat), (void *)(0));
        }

        GLint texcoordAttrib = glGetAttribLocation(drawShader->getProgram(), "TexCoord");
        if (texcoordAttrib != -1) {
            glEnableVertexAttribArray(texcoordAttrib);
            glVertexAttribPointer(texcoordAttrib, 2, GL_FLOAT, GL_FALSE, 12 * sizeof (GLfloat), (void *)(3 * sizeof (GLfloat)));
        }

        GLint normalAttrib = glGetAttribLocation(drawShader->getProgram(), "Normal");
        if (normalAttrib != -1) {
            glEnableVertexAttribArray(normalAttrib);
            glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 12 * sizeof (GLfloat), (void *)(5 * sizeof (GLfloat)));
        }

        GLint tangentAttrib = glGetAttribLocation(drawShader->getProgram(), "Tangent");
        if (normalAttrib != -1) {
            glEnableVertexAttribArray(tangentAttrib);
            glVertexAttribPointer(tangentAttrib, 4, GL_FLOAT, GL_FALSE, 12 * sizeof (GLfloat), (void *)(8 * sizeof (GLfloat)));
        }

        // Our buffers are now safe and secure with OpenGL.
        drawBuffers[i].bufferData.clear();
    }

    return true;
}

bool CWavefrontObj::loadMaterials() {
    if (tmaterials.size()) {
        for (int i = 0; i < drawBuffers.size(); i++) {
            glBindVertexArray(drawBuffers[i].vao);

            // Get material properties from Tiny Object Loader.
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

            // Get textures from Tiny Object Loader.
            QString textureMaps[4];
            textureMaps[0] = tmaterials[i].ambient_texname.c_str();
            textureMaps[1] = tmaterials[i].diffuse_texname.c_str();
            textureMaps[2] = tmaterials[i].specular_texname.c_str();
            textureMaps[3] = tmaterials[i].normal_texname.c_str();

            // Load in textures, or look them up if they have already been loaded.
            GLuint *drawTextureMaps[4] = {&drawBuffers[i].ambientMap, &drawBuffers[i].diffuseMap, &drawBuffers[i].specularMap, &drawBuffers[i].normalMap};
            for (int j = 0; j < 4; j++) {
                if (textureMaps[j].size()) {
                    if (allTextures.contains(textureMaps[j])) {
                        *drawTextureMaps[j] = allTextures[textureMaps[j]];

                        glActiveTexture(GL_TEXTURE0 + j);
                        glBindTexture(GL_TEXTURE_2D, *drawTextureMaps[j]);
                    }
                    else {
                        QImage *image = new QImage(materialPath + textureMaps[j]);
                        if (!image->isNull()) {
                            *image = image->mirrored( 0 , 1 );
                            *image = image->convertToFormat(QImage::Format_RGBA8888);

                            glGenTextures(1, drawTextureMaps[j]);
                            glBindTexture(GL_TEXTURE_2D, *drawTextureMaps[j]);

                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width(), image->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, ((const QImage *)image)->bits());

                            delete image;

                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

                            glGenerateMipmap(GL_TEXTURE_2D);

                            allTextures.insert(textureMaps[j], *drawTextureMaps[j]);
                        }
                    }
                }
            }

            glBindVertexArray(0);
        }
    }

    return true;
}

void CWavefrontObj::drawArrays() {
    glEnable(GL_TEXTURE_2D);

    for (int i = 0; i < drawBuffers.size(); i++) {
        drawShader->useProgram(true);

        drawShader->uniform3f("MatAmbient", drawBuffers[i].matAmbient);
        drawShader->uniform3f("MatDiffuse", drawBuffers[i].matDiffuse);
        drawShader->uniform3f("MatSpecular", drawBuffers[i].matSpecular);
        drawShader->uniform3f("MatTransmittance", drawBuffers[i].matTransmittance);
        drawShader->uniform3f("MatEmission", drawBuffers[i].matEmission);

        drawShader->uniform1f("MatShininess", drawBuffers[i].matShininess);
        drawShader->uniform1f("MatIor", drawBuffers[i].matIor);
        drawShader->uniform1f("MatDissolve", drawBuffers[i].matDissolve);
        drawShader->uniform1f("MatIllum", drawBuffers[i].matIllum);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, drawBuffers[i].ambientMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, drawBuffers[i].diffuseMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, drawBuffers[i].specularMap);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, drawBuffers[i].normalMap);

        drawShader->uniform1ui("AmbientMap", 0);
        drawShader->uniform1ui("DiffuseMap", 1);
        drawShader->uniform1ui("SpecularMap", 2);
        drawShader->uniform1ui("NormalMap", 3);

        glBindVertexArray(drawBuffers[i].vao);
        glDrawArrays(GL_TRIANGLES, 0, drawBuffers[i].count);
    }
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

    allTextures.clear();
}
