#pragma once
#include "../PrimShape.h"
class Rect : public PrimShape
{
    public:
    Rect() { Init(); };
    Rect(GLuint diff, GLuint spec, GLuint normal):PrimShape(diff,spec,normal) { Init(); };
    void Init() override;
    void Draw() override;
    void drawInstance() override;
protected:
    void buildVerticesSmooth() override;
    void buildVerticesFlat() override;
};

