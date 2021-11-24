///////////////////////////////////////////////////////////////////////////////
// Cylinder.h
// ==========
// Cylinder for OpenGL with (base radius, top radius, height, sectors, stacks)
// The min number of sectors (slices) is 3 and the min number of stacks are 1.
// - base radius: the radius of the cylinder at z = -height/2
// - top radius : the radiusof the cylinder at z = height/2
// - height     : the height of the cylinder along z-axis
// - sectors    : the number of slices of the base and top caps
// - stacks     : the number of subdivisions along z-axis
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2018-03-27
// UPDATED: 2019-12-02
///////////////////////////////////////////////////////////////////////////////

#ifndef GEOMETRY_CYLINDER_H
#define GEOMETRY_CYLINDER_H

#include <vector>
#include "../PrimShape.h"

class Cylinder :public PrimShape
{
public:
    // ctor/dtor
    Cylinder(float baseRadius=1.0f, float topRadius=1.0f, float height=1.0f,
             int sectorCount=36, int stackCount=1, bool smooth=true);
    
    Cylinder(GLuint diff, GLuint spec, GLuint normal, float baseRadius = 1.0f, float topRadius = 1.0f, float height = 1.0f,
        int sectorCount = 36, int stackCount = 1, bool smooth = true) : PrimShape(diff, spec, normal) {
        set(baseRadius, topRadius, height, sectorCount, stackCount, smooth);
        Init();
        glm::vec3 norm = { normals.at(0),normals.at(1),normals.at(2) };
        glm::vec3 tan = { tangent.at(0),tangent.at(1),tangent.at(2) };
        glm::vec3 bitan = { bitangent.at(0),bitangent.at(1),bitangent.at(2) };
        glm::mat3 tbn = { bitan,norm,tan };
        glm::vec3 normtest = tbn * glm::normalize(glm::vec3(0.2, 1.0, 0.2));
    };


    // getters/setters
    float getBaseRadius() const             { return baseRadius; }
    float getTopRadius() const              { return topRadius; }
    float getHeight() const                 { return height; }
    int getSectorCount() const              { return sectorCount; }
    int getStackCount() const               { return stackCount; }

    void set(float baseRadius, float topRadius, float height,
             int sectorCount, int stackCount, bool smooth=true);
    void setBaseRadius(float radius);
    void setTopRadius(float radius);
    void setHeight(float radius);
    void setSectorCount(int sectorCount);
    void setStackCount(int stackCount);
    void setSmooth(bool smooth);

    // for indices of base/top/side parts
    unsigned int getBaseIndexCount() const  { return ((unsigned int)indices.size() - baseIndex) / 2; }
    unsigned int getTopIndexCount() const   { return ((unsigned int)indices.size() - baseIndex) / 2; }
    unsigned int getSideIndexCount() const  { return baseIndex; }
    unsigned int getBaseStartIndex() const  { return baseIndex; }
    unsigned int getTopStartIndex() const   { return topIndex; }
    unsigned int getSideStartIndex() const  { return 0; }   // side starts from the begining

    // draw in VertexArray mode
    void drawBase() const;      // draw base cap only
    void drawTop() const;       // draw top cap only
    void drawSide() const;      // draw side only
    void drawLines(const float lineColor[4]) const;     // draw lines only
    void drawWithLines(const float lineColor[4]) const; // draw surface and lines
    
    // debug
    void printSelf() const;
protected:

private:
    // member functions
    void buildVerticesSmooth() override;
    void buildVerticesFlat() override;
    void buildUnitCircleVertices();
    std::vector<float> getSideNormals();

    // memeber vars
    float baseRadius;
    float topRadius;
    float height;
    int sectorCount;                        // # of slices
    int stackCount;                         // # of stacks
    unsigned int baseIndex;                 // starting index of base
    unsigned int topIndex;                  // starting index of top
    bool smooth;
    std::vector<float> unitCircleVertices;
};

#endif
