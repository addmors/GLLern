#pragma once
#include "..\shader\Shad.h"
#include "../Loader.h"
class Water
{
public:

    void Init(int width, int height);

    void loadDuDv(const char* path);

   void drawWater();

   void EnableReflection();
   void DisableReflection();
   
   void EnableRefraction();
   
   void DisableRefraction();

   void addMoveFactor(float deltaTime);
   float getMoveFactor();
   void setSpeedWave(float speed);
   unsigned int getRefractionTexture();

   unsigned int getReflectionTexture();

   
   float getHeight();
   
   void ActiveTexture();

   void WaterDestroy();

private:
     const float planeVertices[18]  {
        // positions           
         1.0f,  1.0f,  
        -1.0f,  1.0f,  
        -1.0f, -1.0f,  
                
         1.0f,  1.0f,  
        -1.0f, -1.0f,  
         1.0f, -1.0f  
    };
     unsigned int planeVAO, planeVBO;
     unsigned int reflectionbuffer;
     unsigned int reflectionTexture;
     unsigned int reflectionRbo;
     unsigned int refractionbuffer;
     unsigned int refractionTexture;
     unsigned int refractionRbo;
     unsigned int dudvMap;
     float waveSpeed = 0.03;
     float moveFactor = 0;
     float height = -7;
};

