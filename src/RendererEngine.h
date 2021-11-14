#pragma once
#include "shader\Shad.h"
#include "Terrian.h"
#include "Camera\Camera.h"
#include "Model\Model.h"
#include "Water\Water.h"
#include"Grass\grass.h"
#include "light\light.h"
#include "gBuffer.h"
#include "Query.h"
#include <vector>
#include<utility>
#include <string>
#include<iterator>


class RendererEngine
{
private:

	Terrian* t;
	Camera* camera;
	Cells* cell;
	PointsLight pointsLight;
	light Light{std::begin(vertices), std::end(vertices), std::begin(indices), std::end(indices)};
	int width_;
	int height_;
	Query query;
	Sphere sphere1{ 1.0f, 36, 18, 1 };
	Sphere sphereLite{ 1.0f, 36, 18, 1 };
	Cylinder cylinder1{ 1, 1, 1, 36, 3, 1 };
	Rect rect1{};
	GBuffer gBuffer{};
	Water water{};
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
	Shader NormalDrawShader{ "src/shader/ShaderDrawNormal.vs","src/shader/ShaderDrawNormal.fs" ,"src/shader/ShaderDrawNormal.gs" };
	Shader shaderGeometryPass{ "src/shader/jastshad.vs", "src/shader/8.1.g_buffer.fs" };
	Shader shaderLightingPass{ "src/shader/shader.vs", "src/shader/8.1.deferred_shading.fs" };
	Shader shaderNullPass{ "src/shader/shader.vs", "src/shader/null_shader.fs" };
	Shader lightShader{ "src/shader/shader.vs", "src/shader/lampshad.fs" };
	
	unsigned int planeVAO;
	unsigned int planeVBO;

	unsigned int quadVAO;
	unsigned int quadVBO;

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

	unsigned int uboMatrices;
	unsigned int cubemapTexture;
	unsigned int cubemapTexture2;

	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	unsigned int rboDepth;

	unsigned int gPosition, gNormal, gAlbedoSpec;

	int time = 0;
	const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
	
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	glm::mat4* projection = nullptr;


public:
	

	RendererEngine(Terrian* _terrain, Camera* _camera, Cells* _cell, int width, int height);
	void configurateDepthBuf();
	void configurateWater(int width, int height, const char* pathDuDvMap);

	void configurateHDR(int width, int height);
	void configureShareds();
	void configureSkyBox();
	bool checkWaterVision();
	void setViewProjection();

	glm::mat4*& getProjection();
	Water& getWater();
	
	void EnableHDR();

	void EnableReflectionWater();

	void EnableRefractionWater();

	void IdleFrameBuffer();
	void DisableHDR();

	void EnableGBuffer();
	void renderInGBuffer(unsigned int texture, unsigned int normal, unsigned int specular, Model* model, int matrixSize);
	void PointLightPass(PointLight&, glm::mat4&);
	void DsPointLightWithSelenticPass();
	void DsFinalPass();
	void SelenticPass(PointLight&, glm::mat4&);
	void MoveLight(float, float);
	void RenderLight();
	
	void renderScene(int width, int height, glm::vec3& cameraPos, glm::mat4& projection, glm::vec3 lightPos, vector<glm::vec3>,
					unsigned int texture, unsigned int normal, unsigned int specular);
	void renderTextureGrass(float time);
	void renderWater(float deltaTime);
	void renderSkyBox(float currFrame);
	void renderModelAnim(Model* model, glm::mat4& trans, float deltaTime,
		std::vector<glm::vec3>& lightPos, glm::vec3& cameraPos, glm::vec3& cameraFront);
	void prepareModelInstanse(vector<glm::mat4>matrix, Model*);
	
	void renderModel(Model* model, std::vector<glm::vec3>& lightPos, int matrixSize);


	void renderInShadow(glm::vec3& PosLight, glm::vec3& dirLight);

	void renderTerrian(int width, int height, glm::vec3& lightPos, std::vector<glm::vec3>& lightsPos);
	void renderHDR(float epsilon, glm::mat4 model = glm::mat4(1.0f));
	void renderGBufferPosition(float, glm::mat4 model = glm::mat4(1.0f));
	void renderReflection(float epsilon, glm::mat4 model = glm::mat4(1.0f));
	void renderRefraction(float epsilon, glm::mat4 model = glm::mat4(1.0f));

	void drawInstanceModel(Model* model, std::vector<glm::vec3>& lightPos, int matrixSize);
	Rect& getBox() { return rect1; };
	Cylinder& getCylinder() { return cylinder1; };
	Sphere& getSphere() { return sphere1; };

	void destroy();
};
