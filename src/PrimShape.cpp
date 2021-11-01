#include "PrimShape.h"


void PrimShape::Init()
{
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (vertices.size() + normals.size() + texCoords.size() + tangent.size() + bitangent.size())
        * sizeof(float), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), &vertices.at(0));
    glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), texCoords.size() * sizeof(float), &texCoords.at(0));

    glBufferSubData(GL_ARRAY_BUFFER, (texCoords.size() + vertices.size()) * sizeof(float),
        normals.size() * sizeof(float), &normals.at(0));
    glBufferSubData(GL_ARRAY_BUFFER, (texCoords.size() + vertices.size() + normals.size()) * sizeof(float),
        tangent.size() * sizeof(float), &tangent.at(0));
    glBufferSubData(GL_ARRAY_BUFFER, (texCoords.size() + vertices.size() + normals.size() + tangent.size()) * sizeof(float),
        bitangent.size() * sizeof(float), &bitangent.at(0));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)(vertices.size() * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
        (GLvoid*)((texCoords.size() + vertices.size()) * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
        (GLvoid*)((texCoords.size() + vertices.size() + normals.size()) * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
        (GLvoid*)((texCoords.size() + vertices.size() + normals.size() + tangent.size()) * sizeof(float)));
    glEnableVertexAttribArray(4);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    prepareInstanse();

}

void PrimShape::AddInstanse()
{
    Instanse.push_back(glm::mat4(1));
}

void PrimShape::draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void PrimShape::drawInstance()
{
    if (Instanse.size() > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, bufferInstance);

        glBufferData(GL_ARRAY_BUFFER, Instanse.size() * sizeof(glm::mat4), &Instanse[0], GL_STATIC_DRAW);

        glBindVertexArray(getVAO());
        glDrawElementsInstanced(
            GL_TRIANGLES, getIndexCount(), GL_UNSIGNED_INT, 0, Instanse.size());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }
}




void PrimShape::buildTangent()
{
    bitangent.resize(normals.size());
    tangent.resize(normals.size());
    for (int i = 0; i < indices.size(); i += 3) {
        float p1 = indices.at(i);
        float p2 = indices.at(i + 1);
        float p3 = indices.at(i + 2);

        glm::vec3 pos1 = glm::vec3(vertices.at(p1 * 3), vertices.at(p1 * 3 + 1), vertices.at(p1 * 3 + 2));
        glm::vec3 pos2 = glm::vec3(vertices.at(p2 * 3), vertices.at(p2 * 3 + 1), vertices.at(p2 * 3 + 2));
        glm::vec3 pos3 = glm::vec3(vertices.at(p3 * 3), vertices.at(p3 * 3 + 1), vertices.at(p3 * 3 + 2));

        glm::vec3 v = pos2 - pos1, w = pos3 - pos1;


        float sx = texCoords.at(p2 * 2) - texCoords.at(p1 * 2), sy = texCoords.at(p2 * 2 + 1) - texCoords.at(p1 * 2 + 1);
        float tx = texCoords.at(p3 * 2) - texCoords.at(p1 * 2), ty = texCoords.at(p3 * 2 + 1) - texCoords.at(p1 * 2 + 1);
        float dirCorrection = (tx * sy - ty * sx) < 0.0f ? -1.0f : 1.0f;

        if (sx * ty == sy * tx) {
            sx = 0.0;
            sy = 1.0;
            tx = 1.0;
            ty = 0.0;
        }

        glm::vec3 tangent1, bitangent1;
        tangent1.x = (w.x * sy - v.x * ty) * dirCorrection;
        tangent1.y = (w.y * sy - v.y * ty) * dirCorrection;
        tangent1.z = (w.z * sy - v.z * ty) * dirCorrection;
        bitangent1.x = (w.x * sx - v.x * tx) * dirCorrection;
        bitangent1.y = (w.y * sx - v.y * tx) * dirCorrection;
        bitangent1.z = (w.z * sx - v.z * tx) * dirCorrection;


        // write into array - for each vertex of the face the same value
        glm::vec3 norm, localTangent, localBitangent;
        norm = { normals.at(p1),normals.at(p1 + 1) ,normals.at(p1 + 2) };

        localTangent = glm::normalize(tangent1 - glm::dot(tangent1, norm));
        localBitangent = glm::normalize(bitangent1 - glm::dot(bitangent1, norm));


        tangent[p1 * 3] = localTangent.x;
        tangent[p1 * 3 + 1] = localTangent.y;
        tangent[p1 * 3 + 2] = localTangent.z;

        bitangent[p1 * 3] = localBitangent.x;
        bitangent[p1 * 3 + 1] = localBitangent.y;
        bitangent[p1 * 3 + 2] = localBitangent.z;
        norm = { normals.at(p1),normals.at(p1 + 1) ,normals.at(p1 + 2) };

        localTangent = glm::normalize(tangent1 - glm::dot(tangent1, norm));
        localBitangent = glm::normalize(bitangent1 - glm::dot(bitangent1, norm));


        tangent[p2 * 3] = localTangent.x;
        tangent[p2 * 3 + 1] = localTangent.y;
        tangent[p2 * 3 + 2] = localTangent.z;

        bitangent[p2 * 3] = localBitangent.x;
        bitangent[p2 * 3 + 1] = localBitangent.y;
        bitangent[p2 * 3 + 2] = localBitangent.z;

        norm = { normals.at(p3 * 3),normals.at(p3 * 3 + 1) ,normals.at(p3 * 3 + 2) };

        localTangent = glm::normalize(tangent1 - glm::dot(tangent1, norm));
        localBitangent = glm::normalize(bitangent1 - glm::dot(bitangent1, norm));


        tangent[p3 * 3] = localTangent.x;
        tangent[p3 * 3 + 1] = localTangent.y;
        tangent[p3 * 3 + 2] = localTangent.z;

        bitangent[p3 * 3] = localBitangent.x;
        bitangent[p3 * 3 + 1] = localBitangent.y;
        bitangent[p3 * 3 + 2] = localBitangent.z;

    }

}

///////////////////////////////////////////////////////////////////////////////
// add single vertex to array
///////////////////////////////////////////////////////////////////////////////
void PrimShape::addVertex(float x, float y, float z)
{
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}



///////////////////////////////////////////////////////////////////////////////
// add single normal to array
///////////////////////////////////////////////////////////////////////////////
void PrimShape::addNormal(float nx, float ny, float nz)
{
    normals.push_back(nx);
    normals.push_back(ny);
    normals.push_back(nz);
}

void PrimShape::addTangent(float tx, float ty, float tz)
{
    tangent.push_back(tx);
    tangent.push_back(ty);
    tangent.push_back(tz);
}

void PrimShape::addBitangent(float bx, float by, float bz)
{

    bitangent.push_back(bx);
    bitangent.push_back(by);
    bitangent.push_back(bz);
}



///////////////////////////////////////////////////////////////////////////////
// add single texture coord to array
///////////////////////////////////////////////////////////////////////////////
void PrimShape::addTexCoord(float s, float t)
{
    texCoords.push_back(s);
    texCoords.push_back(t);
}



///////////////////////////////////////////////////////////////////////////////
// add 3 indices to array
///////////////////////////////////////////////////////////////////////////////
void PrimShape::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}


///////////////////////////////////////////////////////////////////////////////
// dealloc vectors
///////////////////////////////////////////////////////////////////////////////
void PrimShape::clearArrays()
{
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(texCoords);
    std::vector<float>().swap(tangent);
    std::vector<float>().swap(bitangent);
    std::vector<unsigned int>().swap(indices);
    std::vector<unsigned int>().swap(lineIndices);

}


///////////////////////////////////////////////////////////////////////////////
// return face normal of a triangle v1-v2-v3
// if a triangle has no surface (normal length = 0), then return a zero vector
///////////////////////////////////////////////////////////////////////////////
std::vector<float> PrimShape::computeFaceNormal(float x1, float y1, float z1,  // v1
    float x2, float y2, float z2,  // v2
    float x3, float y3, float z3)  // v3
{
    const float EPSILON = 0.000001f;

    std::vector<float> normal(3, 0.0f);     // default return value (0,0,0)
    float nx, ny, nz;

    // find 2 edge vectors: v1-v2, v1-v3
    float ex1 = x2 - x1;
    float ey1 = y2 - y1;
    float ez1 = z2 - z1;
    float ex2 = x3 - x1;
    float ey2 = y3 - y1;
    float ez2 = z3 - z1;

    // cross product: e1 x e2
    nx = ey1 * ez2 - ez1 * ey2;
    ny = ez1 * ex2 - ex1 * ez2;
    nz = ex1 * ey2 - ey1 * ex2;

    // normalize only if the length is > 0
    float length = sqrtf(nx * nx + ny * ny + nz * nz);
    if (length > EPSILON)
    {
        // normalize
        float lengthInv = 1.0f / length;
        normal[0] = nx * lengthInv;
        normal[1] = ny * lengthInv;
        normal[2] = nz * lengthInv;
    }

    return normal;
}


void PrimShape::buildInterleavedVertices()
{
    std::vector<float>().swap(interleavedVertices);

    std::size_t i, j;
    std::size_t count = vertices.size();
    for (i = 0, j = 0; i < count; i += 3, j += 2)
    {
        //interleavedVertices.push_back(vertices[i]);
        //interleavedVertices.push_back(vertices[i+1]);
        //interleavedVertices.push_back(vertices[i+2]);
        interleavedVertices.insert(interleavedVertices.end(), &vertices[i], &vertices[i] + 3);

        //interleavedVertices.push_back(normals[i]);
        //interleavedVertices.push_back(normals[i+1]);
        //interleavedVertices.push_back(normals[i+2]);
        interleavedVertices.insert(interleavedVertices.end(), &normals[i], &normals[i] + 3);

        //interleavedVertices.push_back(texCoords[j]);
        //interleavedVertices.push_back(texCoords[j+1]);
        interleavedVertices.insert(interleavedVertices.end(), &texCoords[j], &texCoords[j] + 2);
    }
}



void PrimShape::prepareInstanse()
{
    glGenBuffers(1, &bufferInstance);
    glBindBuffer(GL_ARRAY_BUFFER, bufferInstance);
    glBindVertexArray(VAO);
    // set attribute pointers for matrix (4 times vec4)
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glVertexAttribDivisor(7, 1);
    glVertexAttribDivisor(8, 1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
};
