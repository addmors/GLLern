#pragma once
#include "shader\Shad.h"
#include "Loader.h"
#include "Terrian.h"
#include "Sphere\Sphere.h"
#include "mCylinder\mCylinder.h"
#include "Camera\Camera.h"
#include "Model\Model.h"
#include <vector>
#include<utility>
#include <string>


unsigned int loadTexture(const char* path);
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
	Shader simpleDepthShader{ "src/shader/3.1.2.shadow_mapping_depth.vs", "src/shader/3.1.2.shadow_mapping_depth.fs" };
	Shader screen{"src/shader/frameshader.vs", "src/shader/frameshader.fs"};
	Shader skyboxShader{ "src/shader/skybox.vs", "src/shader/skybox.fs" };
	Shader objclrShader{ "src/shader/shader3.vs", "src/shader/objvithcolor.fs" };
	Shader ShaderNoBone{ "src/shader/jastshad.vs", "src/shader/objvithcolor.fs" };

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
		
	unsigned int cubemapTexture;
	unsigned int cubemapTexture2;
	int time = 0;
	const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
	int width_;
	int height_;
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 400.0f);;
	
public:
	Terrian* t;
	std::vector<std::pair<std::string, glm::mat4>> bodiesMatrixTrans{};

	void configurateDepthBuf() {
		glGenFramebuffers(1, &depthMapFBO);
		// create depth texture
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void configurateHDR(int width, int height) {
		width_ = width;
		height_ = height;
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glBindVertexArray(0);

		//Frame Buffer;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glGenTextures(1, &textureColorBufferMultiSampled);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width, height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);

		glBindTexture(GL_TEXTURE_2D, 0);


		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glGenFramebuffers(1, &intermediateFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
		// create a color attachment texture
		glGenTextures(1, &screenTexture);
		glBindTexture(GL_TEXTURE_2D, screenTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);
	};
	void configureShareds() {
		//standartShared
		shader.Use();
		shader.SetInt("diffuseTexture", 0);
		shader.SetInt("shadowMap", 1);

		shader.SetVec3("skyColor", 0.7, 0.7, 0.7);
		
		//shaderTerrian
		shaderTerrian.Use();
		shaderTerrian.SetInt("blendMap", 0);
		shaderTerrian.SetInt("backgroundTexture", 1);
		shaderTerrian.SetInt("rTexture", 2);
		shaderTerrian.SetInt("gTexture", 3);
		shaderTerrian.SetInt("bTexture", 4);

		shaderTerrian.SetVec3("skyColor", 0.3, 0.3, 0.3);

		shaderTerrian.SetInt("shadowMap", 5);
		shaderTerrian.SetVec3("fogColor", 0.3, 0.3, 0.3);


		skyboxShader.Use();
		skyboxShader.SetInt("skybox1", 0);

		skyboxShader.SetInt("skybox2", 1);
	}
	void configureSkyBox() {
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
		vector<std::string> faces
		{
			"resources/textures/skybox/clouds1_east.bmp",
			"resources/textures/skybox/clouds1_west.bmp",
			"resources/textures/skybox/clouds1_up.bmp",
			"resources/textures/skybox/clouds1_down.bmp",
			"resources/textures/skybox/clouds1_north.bmp",
			"resources/textures/skybox/clouds1_south.bmp",
		};
		cubemapTexture = loadCubemap(faces);
		vector<std::string> facesNight
		{
			"resources/textures/skybox/blue/bkg1_right.png",
			"resources/textures/skybox/blue/bkg1_left.png",
			"resources/textures/skybox/blue/bkg1_top.png",
			"resources/textures/skybox/blue/bkg1_bot.png",
			"resources/textures/skybox/blue/bkg1_front.png",
			"resources/textures/skybox/blue/bkg1_back.png"
		};
		cubemapTexture2 = loadCubemap(facesNight);
	}
	

	void EnableHDR() {
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	}
	void DisableHDR() {

		glDisable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
		glBlitFramebuffer(0, 0, width_, height_, 0, 0, width_, height_, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void renderScene(int width, int height, glm::vec3& cameraPos, glm::mat4 cameraLook, glm::mat4 projection, glm::vec3 lightPos, unsigned int texture) {
		shader.Use();
		shader.SetMat4("projection", projection);
		shader.SetMat4("view", cameraLook);
		// set light uniforms
		shader.SetVec3("viewPos", cameraPos);
		shader.SetVec3("lightPos", lightPos);
		shader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
		glm::mat4 model = glm::mat4(1.0f);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		// cubes
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.2f, 1.5f, 0.5));
		model = glm::scale(model, glm::vec3(0.5f));
		shader.SetMat4("model", model);
		drawCube();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.5f, 1.0));
		model = glm::scale(model, glm::vec3(0.5f));
		shader.SetMat4("model", model);
		drawCube();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.5f, 2.0));
		model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(0.25));
		shader.SetMat4("model", model);
		drawCube();
		for (auto [name, value] : bodiesMatrixTrans) {

			if (name == "sphere") {
				shader.SetMat4("model", value);
				sphere1.draw();
			}

			if (name == "cylinder") {
				shader.SetMat4("model", value);
				cylinder1.draw();
			}

		}
	};
	void renderSkyBox(glm::mat4& view, glm::mat4& projection, float currFrame) {
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.Use();
		rotation_sky += ROTATION_STEP;
		glm::mat4 my_view = glm::rotate(glm::mat4(glm::mat3(view)), glm::radians(rotation_sky), glm::vec3(0.0, 1.0, 0.0));
		skyboxShader.SetMat4("view", my_view);
		skyboxShader.SetMat4("projection", projection);
		// skybox cube
		time += currFrame/1000;
		time %= 24000;
		unsigned int texture1;
		int texture2;
		float blendFactor;
		if (time >= 0 && time < 5000) {
			texture1 = cubemapTexture2;
			texture2 = cubemapTexture2;
			blendFactor = (static_cast<float>(time) - 0.0f) / (5000.0f - 0.0f);
		}
		else if (time >= 5000 && time < 8000) {
			texture1 = cubemapTexture2;
			texture2 = cubemapTexture;
			blendFactor = (static_cast<float>(time) - 5000.0f) / (8000.0f - 5000.0f);
		}
		else if (time >= 8000 && time < 21000) {
			texture1 = cubemapTexture;
			texture2 = cubemapTexture;
			blendFactor = (static_cast<float>(time) - 8000.0f) / (21000.0f - 8000.0f);
		}
		else {
			texture1 = cubemapTexture;
			texture2 = cubemapTexture2;
			blendFactor = (static_cast<float>(time) - 21000.0f) / (24000.0f - 21000.0f);
		}

		glBindVertexArray(skyboxVAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture2);
		skyboxShader.SetFloat("blendFactor",blendFactor);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default
	}


	void renderModelAnim(Model* model, glm::mat4& trans, glm::mat4 view, float deltaTime, std::vector<glm::vec3>& lightPos) {
		objclrShader.Use();

		objclrShader.Design(view, lightPos);
		objclrShader.SetMat4("projection", projection);
		objclrShader.SetMat4("view", view);
		objclrShader.SetMat4("model", trans);

		if (!model->anim)    //If the object is rigged...
		{
			model->skel.Update();
			for (int i = 0; i < model->meshes.size(); i++) {
				model->meshes[i].bindTexture(objclrShader);
				if (model->meshes[i].sceneLoaderSkeleton.bones.size() > 0) {
					model->meshes[i].SetRigged(objclrShader);
				}
				model->meshes[i].DrawSelf();
			}
		}
		else
		{
			for (int i = 0; i < model->meshes.size(); i++) {
				model->meshes[i].bindTexture(objclrShader);
				model->meshes[i].DrawSelf();
			}
		}

	};
	void renderModel(Model* model, glm::mat4& trans, glm::mat4 view, std::vector<glm::vec3>& lightPos) {
		ShaderNoBone.Use();

		ShaderNoBone.Design(view, lightPos);
		ShaderNoBone.SetMat4("projection", projection);
		ShaderNoBone.SetMat4("view", view);
		ShaderNoBone.SetMat4("model", trans);
		for (int i = 0; i < model->meshes.size(); i++) {
			model->meshes[i].bindTexture(ShaderNoBone);
			model->meshes[i].DrawSelf();
		}
	};

	void drawCube() {
		// initialize (if necessary)
		if (cubeVAO == 0)
		{
			float vertices[] = {
				// back face
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
				 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
				// front face
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
				 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				// left face
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				// right face
				 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
				 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
				// bottom face
				-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
				 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				// top face
				-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
				 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
			};
			glGenVertexArrays(1, &cubeVAO);
			glGenBuffers(1, &cubeVBO);
			// fill buffer
			glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			// link vertex attributes
			glBindVertexArray(cubeVAO);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
		// render Cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	void renderInShadow(glm::vec3 PosLight,glm::vec3 dirLight) {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		float near_plane = 0.1f, far_plane = 110.0f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(PosLight, dirLight, glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// render scene from light's point of view
		simpleDepthShader.Use();
		simpleDepthShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glm::mat4 model = glm::mat4(1.0f);
		auto [scaleX, scaleZ] = t->getScaleTerrian();
		float trans = t->getSize() / 2;
		model = glm::translate(model, glm::vec3(-trans, 0, -trans));
		model = glm::scale(model, glm::vec3(scaleX, 1, scaleZ));
		simpleDepthShader.SetMat4("model", model);
		glDisable(GL_CULL_FACE);
		t->Draw();
		glEnable(GL_CULL_FACE);
		// cubes
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.2f, 1.5f, 0.5));
		model = glm::scale(model, glm::vec3(0.5f));
		simpleDepthShader.SetMat4("model", model);
		drawCube();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.5f, 1.0));
		model = glm::scale(model, glm::vec3(0.5f));
		simpleDepthShader.SetMat4("model", model);
		drawCube();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.5f, 2.0));
		model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(0.25));
		simpleDepthShader.SetMat4("model", model);
		drawCube();
		for (auto [name, value] : bodiesMatrixTrans) {

			if (name == "sphere") {
				simpleDepthShader.SetMat4("model", value);
				sphere1.draw();
			} 

			if (name == "cylinder") {
				simpleDepthShader.SetMat4("model", value);
				cylinder1.draw();
			}
		}
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void renderTerrian(int width, int height, glm::vec3& cameraPos, glm::mat4 model, glm::mat4 cameraLook, glm::mat4 projection, glm::vec3 lightPos) {
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderTerrian.Use();
		shaderTerrian.SetMat4("projection", projection);
		shaderTerrian.SetMat4("view", cameraLook);
		// set light uniforms
		shaderTerrian.SetVec3("viewPos", cameraPos);
		shaderTerrian.SetVec3("lightPos", lightPos);
		shaderTerrian.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
		auto[scaleX,scaleZ]  = t->getScaleTerrian();
		float trans = t->getSize()/2;
		model = glm::translate(model, glm::vec3(-trans, 0.1, -trans));
		model = glm::scale(model, glm::vec3(scaleX, 1, scaleZ));
		shaderTerrian.SetMat4("model", model);
		t->ActiveTexture();
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		t->Draw();
	}


	void renderHDR(float epsilon) {
		screen.Use();
		screen.SetFloat("epsilon", epsilon);
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, screenTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void destroy() {
		glDeleteVertexArrays(1, &quadVAO);
		glDeleteBuffers(1, &quadVBO);
		glDeleteVertexArrays(1, &planeVAO);
		glDeleteBuffers(1, &planeVBO);
		glDeleteVertexArrays(1, &cubeVAO);
		glDeleteBuffers(1, &cubeVBO);
		glDeleteVertexArrays(1, &skyboxVAO);
		glDeleteBuffers(1, &skyboxVBO);

	}

	private: 
		Sphere sphere1{ 1.0f, 36, 18, 1 };
		Cylinder cylinder1{ 1, 1, 1, 36, 3 };
};
