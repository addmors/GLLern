///////////////////////////////////////////////////////////////////////////////
// Sphere.h
// ========
// Sphere for OpenGL with (radius, sectors, stacks)
// The min number of sectors is 3 and The min number of stacks are 2.
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2017-11-01
// UPDATED: 2020-05-20
///////////////////////////////////////////////////////////////////////////////

#ifndef GEOMETRY_SPHERE_H
#define GEOMETRY_SPHERE_H
#include "../PrimShape.h"

class mSphere :public PrimShape
{
public:
    // ctor/dtor
    mSphere(float radius=1.0f, int sectorCount=36, int stackCount=18, bool smooth=true);
    mSphere(GLuint diff, GLuint spec, GLuint norm, float radius = 1.0f, int sectorCount = 36, int stackCount = 18, bool smooth = true);

    
    // getters/setters
    float getRadius() const                 { return radius; }
    int getSectorCount() const              { return sectorCount; }
    int getStackCount() const               { return stackCount; }
    
    void set(float radius, int sectorCount, int stackCount, bool smooth=true);
    void setRadius(float radius);
    void setSectorCount(int sectorCount);
    void setStackCount(int stackCount);
    void setSmooth(bool smooth);


    
    
    // draw in VertexArray mode
    void drawLines(const float lineColor[4]) const;     // draw lines only
    void drawWithLines(const float lineColor[4]) const; // draw surface and lines

    // debug
    void printSelf() const;

protected:

private:
    // member functions
    void buildVerticesSmooth() override;
    void buildVerticesFlat() override;
    

    // memeber vars
    float radius;
    int sectorCount;                        // longitude, # of slices
    int stackCount;                         // latitude, # of stacks
    bool smooth;
    
};

#endif
