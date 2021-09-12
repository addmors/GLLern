#pragma once
#include "shader\Shad.h"


class IntermediateBuffer
{
public:

    IntermediateBuffer();

    ~IntermediateBuffer();

    bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

    void BindForWriting();

    void BindForReading();

private:

    GLuint m_fbo;
    GLuint m_colorBuffer;
    GLuint m_motionBuffer;
    GLuint m_depthBuffer;
};