#pragma once

#include <GL/glew.h>

#define GBUFFER_POSITION_TEXTURE_UNIT 0
#define GBUFFER_DIFFUSE_TEXTURE_UNIT  1
#define GBUFFER_NORMAL_TEXTURE_UNIT   2
#define GBUFFER_TEXCOORD_TEXTURE_UNIT 3

class GBuffer
{
public:

    enum GBUFFER_TEXTURE_TYPE {
        GBUFFER_TEXTURE_TYPE_POSITION,
        GBUFFER_TEXTURE_TYPE_NORMAL,
        GBUFFER_TEXTURE_TYPE_DIFFUSE,
        GBUFFER_TEXTURE_TYPE_FINAL,
        GBUFFER_NUM_TEXTURES
    };

    GBuffer();

    ~GBuffer();

    bool Init(unsigned int WindowWidth, unsigned int WindowHeight);
    
    void CopyMSAA();
    void CopyDepth();
    void StartFrame();
    void BindForGeomPass();
    void BindForStencilPass();
    void BindForLightPass();
    void BindForFinalPass();
    GLuint getPositionsTexture();
    GLuint getNormalTexture();
    GLuint getAlbedoTexture();
    GLuint getFinalTexture();



private:
    GLuint width;
    GLuint height;
    GLuint m_fbo;
    GLuint m_textures[GBUFFER_NUM_TEXTURES];
    GLuint m_texturesMS[GBUFFER_NUM_TEXTURES];
    GLuint m_depthTexture;
    GLuint m_depthTextureInter;
    GLuint m_finalTexture;
    GLuint intermediateFBO;
};

