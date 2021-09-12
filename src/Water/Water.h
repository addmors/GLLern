#pragma once
#include "..\shader\Shad.h"
#include "../Loader.h"
class Water
{
public:

   void Init(int width, int height) {

        glGenVertexArrays(1, &planeVAO);
        glGenBuffers(1, &planeVBO);
        glBindVertexArray(planeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
       
        
        glGenFramebuffers(1, &reflectionbuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, reflectionbuffer);
   

        glGenTextures(1, &reflectionTexture);
        glBindTexture(GL_TEXTURE_2D, reflectionTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        // присоедиение текстуры к объекту текущего кадрового буфера
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflectionTexture, 0);
        
        glGenRenderbuffers(1, &reflectionRbo);
        glBindRenderbuffer(GL_RENDERBUFFER, reflectionRbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, reflectionRbo);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer for Water Reflection is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenFramebuffers(1, &refractionbuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, refractionbuffer);


        glGenTextures(1, &refractionTexture);
        glBindTexture(GL_TEXTURE_2D, refractionTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        // присоедиение текстуры к объекту текущего кадрового буфера
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, refractionTexture, 0);

        glGenRenderbuffers(1, &refractionRbo);
        glBindRenderbuffer(GL_RENDERBUFFER, refractionRbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, refractionRbo);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer for Water Refraction is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
   }

   void loadDuDv(const char* path) {
       dudvMap = loadTexture(path);
   }

   void drawWater() {
       glBindVertexArray(planeVAO);
       glDrawArrays(GL_TRIANGLES, 0, 6);
       glBindVertexArray(0);
   };

   void EnableReflection() {
       glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
       glBindFramebuffer(GL_FRAMEBUFFER, reflectionbuffer);
   }
   
   void DisableReflection() {
       glBindFramebuffer(GL_FRAMEBUFFER, 0); // возвращаем буфер кадра по умолчанию
   }
   
   void EnableRefraction() {
       glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
       glBindFramebuffer(GL_FRAMEBUFFER, refractionbuffer);
       

   }
   
   void DisableRefraction() {
       glBindFramebuffer(GL_FRAMEBUFFER, 0); // возвращаем буфер кадра по умолчанию
  
   }
   void addMoveFactor(float deltaTime) {
       moveFactor = fmod(moveFactor+waveSpeed*deltaTime, 1);
   }
   float getMoveFactor() {
       return moveFactor;
   }
   void setSpeedWave(float speed) {
       waveSpeed = speed;
   };
   unsigned int getRefractionTexture() {
       return refractionTexture;
   }

   unsigned int getReflectionTexture() {
       return reflectionTexture;
   }

   
   float getHeight() { 
       return height;
   };
   
   void ActiveTexture() {
       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D, reflectionTexture);

       glActiveTexture(GL_TEXTURE1);
       glBindTexture(GL_TEXTURE_2D, refractionTexture);

       glActiveTexture(GL_TEXTURE2);
       glBindTexture(GL_TEXTURE_2D, dudvMap);
   };

   void WaterDestroy() {
       glDeleteVertexArrays(1, &planeVAO);
       glDeleteBuffers(1, &planeVBO);
   }

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

