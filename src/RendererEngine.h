#pragma once
#include "shader\Shad.h"
#include "Loader.h"
#include "Terrian.h"
#include "Sphere\Sphere.h"
#include "mCylinder\mCylinder.h"
#include "Camera\Camera.h"
#include "Model\Model.h"
#include "Water\Water.h"
#include"Grass\grass.h"
#include "Query.h"
#include <vector>
#include<utility>
#include <string>


static GLfloat vertices[] = {
	//Первый квадрат в кубе 
	// Позиции           //Нормаль         // Текстурные координаты
	 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f,   // Верхний правый
	 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f,   // Нижний правый
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,   // Нижний левый
	-0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f,   // Верхний левый
	//Второй квадрат в кубе 
	// Позиции          //Нормаль             // Текстурные координаты
	 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,    1.0f, 1.0f,   // Верхний правый
	 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,    1.0f, 0.0f,   // Нижний правый
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,    0.0f, 0.0f,   // Нижний левый
	-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,    0.0f, 1.0f,    // Верхний левый
	//Третий квадрат в кубе 
	// Позиции          // Нормаль           // Текстурные координаты
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,    1.0f, 1.0f,   // Верхний правый
	 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
	 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // Нижний левый
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,   0.0f, 1.0f,    // Верхний левый
	 //Четвёртый квадрат в кубе 
	// Позиции          // Нормаль             // Текстурные координаты
	 -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
	 -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
	 -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // Нижний левый
	 -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,    // Верхний левый
	 //Пятый квадрат в кубе 
	// Позиции          // Нормаль             // Текстурные координаты
	  0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  1.0f, 1.0f,   // Верхний правый
	  0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,   // Нижний правый
	 -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f,   // Нижний левый
	 -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f,    // Верхний левый
	//Шестой квадрат в кубе
	// Позиции          // Нормаль             // Текстурные координаты
	  0.5f,  -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
	  0.5f,  -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  1.0f, 0.0f,   // Нижний правый
	 -0.5f,  -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  0.0f, 0.0f,   // Нижний левый
	 -0.5f,  -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,  0.0f, 1.0f    // Верхний левый
};

static GLfloat quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	 // positions   // texCoords
	 -1.0f,  1.0f,  0.0f, 1.0f,
	 -1.0f, -1.0f,  0.0f, 0.0f,
	  1.0f, -1.0f,  1.0f, 0.0f,

	 -1.0f,  1.0f,  0.0f, 1.0f,
	  1.0f, -1.0f,  1.0f, 0.0f,
	  1.0f,  1.0f,  1.0f, 1.0f
};

static GLuint indices[] = {  // Помните, что мы начинаем с 0!
0, 1, 3,   // Первый треугольник
1, 2, 3,    // Второй треугольник //1 кв
4, 5, 7,
5, 6, 7, //2кв
8, 9,11,
9,10,11, //3кв
12,13,15,
13,14,15, //4кв
16,17,19,
17,18,19, //5кв
20,21,23,
21,22,23 //6кв
};

static float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

class RendererEngine
{
private:
	std::vector<Model*> models; 
	Shader shader{ "src/shader/3.1.2.shadow_mapping.vs", "src/shader/3.1.2.shadow_mapping.fs" };
	Shader shaderTerrian{ "src/shader/terrianShader.vs", "src/shader/terrianShader.fs" };
	Shader simpleDepthShader{ "src/shader/3.1.2.shadow_mapping_depth.vs", "src/shader/3.1.2.shadow_mapping_depth.fs"};
	Shader screen{"src/shader/frameshader.vs", "src/shader/frameshader.fs"};
	Shader skyboxShader{ "src/shader/skybox.vs", "src/shader/skybox.fs" };
	Shader objclrShader{ "src/shader/shader3.vs", "src/shader/objvithcolor.fs"};
	Shader ShaderNoBone{ "src/shader/jastshad.vs", "src/shader/objvithcolor.fs" };
	Shader WaterShader{ "src/shader/WaterShader.vs", "src/shader/WaterShader.fs" };
	Shader TextureGrassShader{"src/shader/textureGrass.vs", "src/shader/textureGrass.fs"};
	Shader OccluderShader{"src/shader/occluder.vs","src/shader/occluder.fs"};
	Shader OccluderWaterShader{ "src/shader/occluderWater.vs","src/shader/occluderWater.fs" };
	float rotation_sky = 0;
	float ROTATION_STEP = 0.01f;
	
	unsigned int planeVAO;
	unsigned int planeVBO;

	unsigned int quadVAO;
	unsigned int quadVBO;

	unsigned int cubeVBO;
	unsigned int cubeVAO = 0;

	unsigned int skyboxVAO;
	unsigned int skyboxVBO;

	unsigned int depthMapFBO;
	unsigned int depthMap;

	unsigned int framebuffer;
	unsigned int textureColorBufferMultiSampled;
	unsigned int rbo;

	unsigned int screenTexture;
	unsigned int intermediateFBO;

	unsigned int bufferTree;
	unsigned int cubemapTexture;
	unsigned int cubemapTexture2;
	int time = 0;
	const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
	int width_;
	int height_;
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	glm::mat4* projection = nullptr;


public:
	
	Terrian* t;
	Cells* cell;
	Query query;
	Camera* camera;
	std::vector<std::pair<std::string, glm::mat4>> bodiesMatrixTrans{};

	void configurateDepthBuf();
	void configurateWater(int width, int height, const char* pathDuDvMap);

	void configurateHDR(int width, int height);
	void configureShareds();
	void configureSkyBox();
	bool checkWaterVision();
	
	glm::mat4*& getProjection();
	Water& getWater();
	
	void EnableHDR();

	void EnableReflectionWater();

	void EnableRefractionWater();
	void IdleFrameBuffer();
	void DisableHDR();
	
	void renderScene(int width, int height, glm::vec3& cameraPos, glm::mat4& projection, glm::vec3 lightPos, unsigned int texture);
	void renderTextureGrass(float time);
	void renderWater(float deltaTime);
	void renderSkyBox(float currFrame);
	void renderModelAnim(Model* model, glm::mat4& trans, float deltaTime,
		std::vector<glm::vec3>& lightPos, glm::vec3& cameraPos, glm::vec3& cameraFront);
	void prepareModelInstanse(vector<glm::mat4>matrix, Model*);
	void renderModel(Model* model, std::vector<glm::vec3>& lightPos, int matrixSize,
		glm::vec3& cameraPos,glm::vec3& cameraFront);

	void drawCube();

	void renderInShadow(glm::vec3& PosLight, glm::vec3& dirLight);

	void renderTerrian(int width, int height, glm::vec3& cameraPos, glm::vec3& cameraFront, glm::mat4& projection, glm::vec3& lightPos, std::vector<glm::vec3>& lightsPos);
	void renderHDR(float epsilon, glm::mat4 model = glm::mat4(1.0f));
	void renderReflection(float epsilon, glm::mat4 model = glm::mat4(1.0f));
	void renderRefraction(float epsilon, glm::mat4 model = glm::mat4(1.0f));

	void destroy();

	private: 
		Sphere sphere1{ 1.0f, 36, 18, 1 };
		Cylinder cylinder1{ 1, 1, 1, 36, 3 };
		Water water{};

};
