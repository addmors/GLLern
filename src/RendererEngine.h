#pragma once
#include "shader\Shad.h"
#include "Terrian.h"
#include "Sphere\Sphere.h"
#include "mCylinder\mCylinder.h"
#include <vector>
#include<utility>

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


class RendererEngine
{
public:
	unsigned int planeVAO, planeVBO,
		depthMapFBO, depthMap,
		quadVAO, quadVBO,
		framebuffer, texColorBuffer,
		rbo, cubeVAO = 0, cubeVBO;
	const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
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

		glGenTextures(1, &texColorBuffer);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		// присоедиение текстуры к объекту текущего кадрового буфера
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	};

	void renderScene(int width, int height, Shader* shader, glm::vec3& cameraPos, glm::mat4 cameraLook, glm::mat4 projection, glm::vec3 lightPos, unsigned int texture) {
		shader->Use();
		shader->SetMat4("projection", projection);
		shader->SetMat4("view", cameraLook);
		// set light uniforms
		shader->SetVec3("viewPos", cameraPos);
		shader->SetVec3("lightPos", lightPos);
		shader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
		glm::mat4 model = glm::mat4(1.0f);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		// cubes
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.2f, 1.5f, 0.5));
		model = glm::scale(model, glm::vec3(0.5f));
		shader->SetMat4("model", model);
		drawCube();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.5f, 1.0));
		model = glm::scale(model, glm::vec3(0.5f));
		shader->SetMat4("model", model);
		drawCube();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.5f, 2.0));
		model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(0.25));
		shader->SetMat4("model", model);
		drawCube();
		for (auto [name, value] : bodiesMatrixTrans) {

			if (name == "sphere") {
				shader->SetMat4("model", value);
				sphere1.draw();
			}

			if (name == "cylinder") {
				shader->SetMat4("model", value);
				cylinder1.draw();
			}

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

	void renderInShadow(glm::vec3 PosLight, Shader* simpleDepthShader) {
		float near_plane = 0.1f, far_plane = 100.0f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(PosLight, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// render scene from light's point of view
		simpleDepthShader->Use();
		simpleDepthShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glm::mat4 model = glm::mat4(1.0f);
		simpleDepthShader->SetMat4("model", model);
		t->Draw();
		// cubes
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.2f, 1.5f, 0.5));
		model = glm::scale(model, glm::vec3(0.5f));
		simpleDepthShader->SetMat4("model", model);
		drawCube();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.5f, 1.0));
		model = glm::scale(model, glm::vec3(0.5f));
		simpleDepthShader->SetMat4("model", model);
		drawCube();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.5f, 2.0));
		model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(0.25));
		simpleDepthShader->SetMat4("model", model);
		drawCube();
		for (auto [name, value] : bodiesMatrixTrans) {

			if (name == "sphere") {
				simpleDepthShader->SetMat4("model", value);
				sphere1.draw();
			} 

			if (name == "cylinder") {
				simpleDepthShader->SetMat4("model", value);
				cylinder1.draw();
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void EnableHDR() {
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	}
	void DisableHDR() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void renderTerrian(int width, int height, Shader* shader, glm::vec3& cameraPos, glm::mat4 model, glm::mat4 cameraLook, glm::mat4 projection, glm::vec3 lightPos, unsigned int texture ) {
		shader->Use();
		shader->SetMat4("projection", projection);
		shader->SetMat4("view", cameraLook);
		// set light uniforms
		shader->SetVec3("viewPos", cameraPos);
		shader->SetVec3("lightPos", lightPos);
		shader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
		auto[scaleX,scaleZ]  = t->getScaleTerrian();
		float trans = t->getSize()/2;
		model = glm::translate(model, glm::vec3(-trans, 0.1, -trans));
		model = glm::scale(model, glm::vec3(scaleX, 1, scaleZ));
		shader->SetMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		t->Draw();
	}


	void renderHDR(Shader* screen, float epsilon) {

		screen->Use();
		screen->SetFloat("epsilon", epsilon);
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
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


	}
	private: 
		Sphere sphere1{ 1.0f, 36, 18, 1 };
		Cylinder cylinder1{ 1, 1, 1, 36, 3 };
};

