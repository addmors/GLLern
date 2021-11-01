#pragma once

#include "PrimShape.h"
class Rect : public PrimShape
{
    public:
    Rect() { Init(); 
    };
    void Init() override;
    void draw() override;
    void drawInstance() override;
private:
    void buildVerticesSmooth() override;
    void buildVerticesFlat() override;
};

