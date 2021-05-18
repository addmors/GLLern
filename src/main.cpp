#define GLEW_STATIC
#include <string>
#include <iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include <stb_image.h>
#include<btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Precompile.h"
#include"shader/Shad.h"
#include "light/light.h"
#include"Camera/Camera.h"
#include <stdlib.h>
#include<SOIL.h>
#include"Model/Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Player\Player.h"
#include "Sphere\Sphere.h"
#include "mCylinder\mCylinder.h"
#include "pEngine\pEngine.h"

glm::vec3 skaling = glm::vec3(0.1f, 0.1f, 0.1f);
glm::vec3 cameraPos = glm::vec3(3.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(-1.0f, -0.0f, -0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat deltaTime = 0.0f;	// Время, прошедшее между последним и текущим кадром
GLfloat lastFrame = 0.0f;  	// Время вывода последнего кадра
GLfloat yaw = -135.0f;
GLfloat pitch = 0.0f;
GLfloat fov = 45.0f;
GLfloat lastX = 400, lastY = 300; //Координаты мышки;
float epsilon = 0.4;


Camera camera(cameraPos, cameraFront, cameraUp, fov);
pEngine* pengine;
Model ourModel;

void key_callback_for_movement(int key, int action);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char* path);
void renderScene(Shader& shader);
void renderCube();
void renderQuad();
glm::mat4 getInbtTransform(btTransform& t);

unsigned int planeVAO;

int main() {
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//Минорная
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Установка профайла для которого создается контекст
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Выключение возможности изменения размера окна
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "LearnOpenGL", nullptr/*glfwGetPrimaryMonitor()*/, nullptr);
	if (window == nullptr)
	{
		fprintf(stderr,"Failed to create GLFW window");	
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW");
		return -1;
	}
	glfwSetCursorPos(window, lastX, lastY);
	glfwSetScrollCallback(window, scroll_callback); //активаци колесика мыши
	glfwSetKeyCallback(window, key_callback);// функции по нажатию.
	glfwSetCursorPosCallback(window, mouse_callback); //Функции по движению мыши
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Активация курсора скрыть указатель 
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);	
	glViewport(0, 0, width, height);
	
	



	Shader shader("src/shader/3.1.2.shadow_mapping.vs", "src/shader/3.1.2.shadow_mapping.fs");
	Shader simpleDepthShader("src/shader/3.1.2.shadow_mapping_depth.vs", "src/shader/3.1.2.shadow_mapping_depth.fs");
	Shader lightShader("src/shader/shader.vs", "src/shader/lampshad.fs");
	Shader objclrShader("src/shader/shader3.vs", "src/shader/objvithcolor.fs");
	Shader screen("src/shader/frameshader.vs", "src/shader/frameshader.fs");

	
	unsigned int woodTexture = loadTexture("wood.png");
	ourModel.loadModel("resources/objects/Black Dragon NEW/ganfaul_m_aure.dae");

	ourModel.loadIdleAnimaitons("resources/objects/Black Dragon NEW/idle.dae");
	ourModel.loadAnims("resources/objects/Black Dragon NEW/Walking.dae","Walking");
	ourModel.loadAnims("resources/objects/Black Dragon NEW/Walking Backwards.dae","WalkBackWard");

	pengine = new pEngine();

	Player persone(ourModel, camera, objclrShader, pengine);
	//get key
	camera.keys = &Key.keys;
	ourModel.keys = &Key.keys;
	persone.keys = &Key.keys;


	//vertex info
	GLfloat vertices[] = {
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
	GLuint indices[] = {  // Помните, что мы начинаем с 0!
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
	float planeVertices[] = {
		// positions            // normals         // texcoords
		 25.0f, -0.0f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.0f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-25.0f, -0.0f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
				   
		 25.0f, -0.0f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.0f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		 25.0f, -0.0f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
	};
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		 // positions   // texCoords
		 -1.0f,  1.0f,  0.0f, 1.0f,
		 -1.0f, -1.0f,  0.0f, 0.0f,
		  1.0f, -1.0f,  1.0f, 0.0f,

		 -1.0f,  1.0f,  0.0f, 1.0f,
		  1.0f, -1.0f,  1.0f, 0.0f,
		  1.0f,  1.0f,  1.0f, 1.0f
	};


	// plane VAO
	unsigned int planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);

	

	// configure depth map FBO
	// -----------------------
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// shader configuration
	// --------------------
	shader.Use();
	shader.SetInt("diffuseTexture", 0);
	shader.SetInt("shadowMap", 1);
	
	
	glm::vec3 lightPosForShad(-2.0f, 4.0f, -1.0f);


	
	//Frame texture	
	unsigned int quadVAO, quadVBO;
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
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	unsigned int texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, 0);

	// присоедиение текстуры к объекту текущего кадрового буфера
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);


	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, 0); 
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//End Frame buffer;


	vector<glm::vec3> lightPos;

	lightPos.push_back(glm::vec3(4.0f,4.0f,4.0f));
	lightPos.push_back(glm::vec3(-4.0f, 4.0f, 4.0f));
	lightPos.push_back(glm::vec3(4.0f, 4.0f, -4.0f));
	lightPos.push_back(glm::vec3(-4.0f, 4.0f, -4.0f));

	light Light(std::begin(vertices),std::end(vertices), std::begin(indices),std::end(indices), lightPos);

	Sphere sphere1(1.1f, 36, 18, false);
	Cylinder cylinder1(1, 1, 1, 36, 3);
	pengine->addSphere(1.0,0,20,0,1.0);
	pengine->addCylinder(2, 5, 0, 30, 0, 1.0);
	
	while (!glfwWindowShouldClose(window))
	{
		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glm::mat4 model = glm::mat4();
		GLfloat currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		pengine->step(deltaTime);
		camera.deltaTime = deltaTime;
		lastFrame = currentFrame;
		camera.do_movement();


		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 7.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPosForShad, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// render scene from light's point of view
		simpleDepthShader.Use();
		simpleDepthShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		renderScene(simpleDepthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.Use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)width / (float)height, 0.1f, 100.0f);
		glm::mat4 view = camera.LoocAt();
		shader.SetMat4("projection", projection);
		shader.SetMat4("view", view);
		// set light uniforms
		shader.SetVec3("viewPos", camera.cameraPos);
		shader.SetVec3("lightPos", lightPosForShad);
		shader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		renderScene(shader);



		objclrShader.Use();
		view = camera.LoocAt();
		projection = glm::perspective(glm::radians(camera.fov), (float)width / (float)height, 0.1f, 100.0f);
		btTransform t;
		t = persone.charCon->getGhostObject()->getWorldTransform();
		glm::mat4 glmatrix = getInbtTransform(t);
		//model = glm::translate(model, glm::vec3(camera.objPos.x,0, camera.objPos.z)); // translate it down so it's at the center of the scene
		//if(Key.keys[67])
			//glmatrix = glm::rotate(glmatrix, glm::radians(90.0f - camera.yaw), glm::vec3(0.0, 1.0, 0.0));

		//model = glm::scale(model,skaling);

		objclrShader.Design(view, lightPos);
		
		objclrShader.SetMat4("projection", projection);
		objclrShader.SetMat4("view", view);
		objclrShader.SetMat4("model", glmatrix);
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		persone.update();
		persone.show();
		persone.Persone->PlayAnimation(deltaTime);
		
		for (auto& a : pengine->getBody()) {
			if (a->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE) {
				pengine->RenderSphere(a,&sphere1, view, projection);
			}
			if (a->getCollisionShape()->getShapeType() == CYLINDER_SHAPE_PROXYTYPE) {
				pengine->RenderCylinder(a, &cylinder1, view, projection);
			}

		};
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		Light.UseLight(view, projection, lightPos);	

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // возвращаем буфер кадра по умолчанию
		
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		screen.Use();
		screen.SetFloat("epsilon", epsilon);
		glBindVertexArray(quadVAO);	
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	Light.Delete();
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
	persone.destroy();
	for (auto& a : ourModel.meshes) {
		a.Delete();
	}
	delete pengine;
	glfwTerminate();
	return 0;

}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true, 
	// и приложение после этого закроется
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		epsilon += 0.1;
	}
	if (key == GLFW_KEY_V && action == GLFW_PRESS) {
		epsilon -= 0.1;
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		
		btRigidBody* sphere = pengine->addSphere(1,camera.objPos.x, camera.objPos.y, camera.objPos.z,1);
		sphere->setLinearVelocity(btVector3(camera.cameraFront.x*20, camera.cameraFront.y*20, camera.cameraFront.z*20));
	}
	else {
		key_callback_for_movement(key, action);
		ourModel.key_callback(key, action);
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	camera.mouse_callback(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (Key.keys[67]) {
		skaling += (yoffset * 0.1);
	} 
	else camera.scroll_callback(xoffset, yoffset);
}

void key_callback_for_movement(int key, int action) {
	Key.keys[key] = action;
	if (action == 1)  std::cout << key;
};

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}



void renderScene(Shader& shader)
{
	// floor
	glm::mat4 model = glm::mat4(1.0f);
	shader.SetMat4("model", model);
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// cubes
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.2f, 1.5f, 0.5));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetMat4("model", model);
	renderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.5f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetMat4("model", model);
	renderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.5f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	shader.SetMat4("model", model);
	renderCube();
}

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
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

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

glm::mat4 getInbtTransform(btTransform& t) {
	float mat[16];
	t.getOpenGLMatrix(mat);
	return { mat[0], mat[1], mat[2], mat[3],
		mat[4], mat[5], mat[6], mat[7],
		mat[8], mat[9], mat[10], mat[11],
		mat[12], mat[13], mat[14], mat[15]};
}