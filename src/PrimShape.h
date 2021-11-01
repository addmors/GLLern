#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader/Shad.h"



class PrimShape
{

public:
    PrimShape(bool smooth = false) : interleavedStride(32) {};
    virtual void Init();
    void AddInstanse();
    unsigned int getVertexCount() const { return (unsigned int)vertices.size() / 3; }
    unsigned int getNormalCount() const { return (unsigned int)normals.size() / 3; }
    unsigned int getTexCoordCount() const { return (unsigned int)texCoords.size() / 2; }
    unsigned int getIndexCount() const { return (unsigned int)indices.size(); }
    unsigned int getLineIndexCount() const { return (unsigned int)lineIndices.size(); }
    unsigned int getTriangleCount() const { return getIndexCount() / 3; }
    unsigned int getVertexSize() const { return (unsigned int)vertices.size() * sizeof(float); }
    unsigned int getNormalSize() const { return (unsigned int)normals.size() * sizeof(float); }
    unsigned int getTexCoordSize() const { return (unsigned int)texCoords.size() * sizeof(float); }
    unsigned int getIndexSize() const { return (unsigned int)indices.size() * sizeof(unsigned int); }
    unsigned int getLineIndexSize() const { return (unsigned int)lineIndices.size() * sizeof(unsigned int); }
    const float* getVertices() const { return vertices.data(); }
    const float* getNormals() const { return normals.data(); }
    const float* getTexCoords() const { return texCoords.data(); }
    const unsigned int* getIndices() const { return indices.data(); }
    const unsigned int* getLineIndices() const { return lineIndices.data(); }


    // for interleaved vertices: V/N/T
    unsigned int getInterleavedVertexCount() const { return getVertexCount(); }    // # of vertices
    unsigned int getInterleavedVertexSize() const { return (unsigned int)interleavedVertices.size() * sizeof(float); }    // # of bytes
    int getInterleavedStride() const { return interleavedStride; }   // should be 32 bytes
    const float* getInterleavedVertices() const { return interleavedVertices.data(); }

    virtual void draw();
    virtual void drawInstance();
    void prepareInstanse();
    std::vector<glm::mat4>& getInstanse() {return Instanse; };

    ~PrimShape() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    GLuint getVAO() { return VAO; };

protected: 


    void clearArrays();

    virtual void buildVerticesSmooth() = 0;
    virtual void buildVerticesFlat() = 0;
    void buildTangent();

    void buildInterleavedVertices();
    void addVertex(float x, float y, float z);
    void addNormal(float x, float y, float z);
    void addTangent(float x, float y, float z);
    void addBitangent(float x, float y, float z);
    void addTexCoord(float s, float t);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);


    std::vector<float> computeFaceNormal(float x1, float y1, float z1,  // v1
        float x2, float y2, float z2,  // v2
        float x3, float y3, float z3); // v3

    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> tangent;
    std::vector<float> bitangent;

    std::vector<float> texCoords;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> lineIndices;

    // interleaved
    std::vector<float> interleavedVertices;
    int interleavedStride;                  // # of bytes to hop to the next vertex (should be 32 bytes)
    GLuint VBO = 0;
    GLuint EBO = 0;
    GLuint VAO = 0;
    GLuint bufferInstance = 0;
    std::vector<glm::mat4> Instanse{};
};

