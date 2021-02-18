#define GLEW_STATIC
#include <string>
#include <iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"shader/Shad.h"
#include "light/light.h"
#include"Camera/Camera.h"
#include <stdlib.h>
#include<SOIL.h>
#include"Model/Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

glm::vec3 skaling = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 cameraPos = glm::vec3(10.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(-1.0f, -0.0f, -0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
bool keys[1024];
GLfloat deltaTime = 0.0f;	// Время, прошедшее между последним и текущим кадром
GLfloat lastFrame = 0.0f;  	// Время вывода последнего кадра
GLfloat yaw = -135.0f;
GLfloat pitch = 0.0f;
GLfloat fov = 45.0f;
GLfloat lastX = 400, lastY = 300; //Координаты мышки;
static Animation Anim_Test_Idle2("", FramesToTime(glm::vec2(0, 40)), 1);
Camera camera(cameraPos, cameraFront, cameraUp, fov, yaw, pitch);
void key_callback_for_movement(int key, int action);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


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
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr/*glfwGetPrimaryMonitor()*/, nullptr);
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
	
	

	Shader shader("src/shader/shader3.vs", "src/shader/objvithcolor.fs");
	Shader lightShader("src/shader/shader.vs", "src/shader/lampshad.fs");
	Shader objclrShader("src/shader/shader3.vs", "src/shader/objvithcolor.fs");
	Shader screen("src/shader/shadowshader.vs", "src/shader/shadowshader.fs");
	Model ourModel("resources/objects/myne/mango2.dae");

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

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		 // positions   // texCoords
		 -1.0f,  1.0f,  0.0f, 1.0f,
		 -1.0f, -1.0f,  0.0f, 0.0f,
		  1.0f, -1.0f,  1.0f, 0.0f,

		 -1.0f,  1.0f,  0.0f, 1.0f,
		  1.0f, -1.0f,  1.0f, 0.0f,
		  1.0f,  1.0f,  1.0f, 1.0f
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

	int n = sizeof(vertices);
	GLuint VBO;
	GLuint EBO;
	GLuint VAO;
	
	int w, h;
	GLuint texture1;
	glGenTextures(1, &texture1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char* image = SOIL_load_image("wood.png", &w, &h, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	SOIL_free_image_data(image);
	GLuint texture2;
	glGenTextures(1, &texture2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char* image2 = SOIL_load_image("rama.png", &w, &h, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

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




	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	unsigned int texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// присоедиение текстуры к объекту текущего кадрового буфера
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);


	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, 0); 
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// Так как VBO объекта-контейнера уже содержит все необходимые данные, то нам нужно только связать с ним новый VAO 
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Настраиваем атрибуты (нашей лампе понадобятся только координаты вершин)	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	*/
	
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(5.0f, 5.0f, 5.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	vector<glm::vec3> lightPos;
	glm::mat4 projection;
	glm::mat4 model = glm::mat4();
	glm::mat4 view;
	for (int i = 1; i < 40; i++) {
		lightPos.push_back(glm::vec3(i, 0, 0));
		lightPos.push_back(glm::vec3(0, i, 0));
		lightPos.push_back(glm::vec3(0, 0, i));
		lightPos.push_back(glm::vec3(-i, 0, 0));
		lightPos.push_back(glm::vec3(0, -i, 0));
		lightPos.push_back(glm::vec3(0, 0, -i));
	}
	light Light(std::begin(vertices),std::end(vertices), std::begin(indices),std::end(indices), lightPos);
	//screen.Use();
	//screen.SetInt("screenTexture", 0);
	//objclrShader.Use();
	//objclrShader.SetInt("material.texture_diffuse1", 0);
	//objclrShader.SetInt("material.texture_specular1", 1);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		GLfloat currentFrame = glfwGetTime();
		camera.deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		camera.do_movement();
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		objclrShader.Use();
		view = camera.LoocAt();
		projection = glm::perspective(glm::radians(camera.fov), (float)width / (float)height, 0.1f, 100.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model,skaling);	// it's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 0, 1));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
	/*	shader.Use();
		shader.SetMat4("projection",projection);	
		shader.SetMat4("model", model);
		shader.SetMat4("view", view);
		shader.Design(view, lightPos);
		ourModel.DrawAnim(shader, model, view, projection);*/

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture1);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture2);
		//objclrShader.SetFloat("material.shininess", 0.4f*128.0f);
		objclrShader.Design(view, lightPos);

		objclrShader.SetMat4("projection", projection);
		objclrShader.SetMat4("view", view);
		objclrShader.SetMat4("model", model);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		ourModel.DrawAnim(objclrShader, model, view, projection);

		//glBindVertexArray(VAO);
		//for (GLuint i = 0; i < sizeof(cubePositions) / sizeof(glm::vec3); i++) {
		//	glm::mat4 model;
		//	model = glm::translate(model, cubePositions[i]);
		//	GLfloat angle = glm::pi<float>() / 4;
		//	//model = glm::rotate(model, angle*i, glm::vec3(1.0f, 1.0f, 0.0f));
		//	objclrShader.SetMat4("model", model);
		//	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//}
		//glm::mat4 model;
		//model = glm::translate(model, glm::vec3(-10,0,0));
		//GLfloat angle = glm::pi<float>() / 4;
		////model = glm::rotate(model, angle*i, glm::vec3(1.0f, 1.0f, 0.0f));
		//objclrShader.SetMat4("model", model);
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//glBindVertexArray(0);
		//Light.UseLight(view,projection,lightPos);
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // возвращаем буфер кадра по умолчанию
		
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		screen.Use();
		glBindVertexArray(quadVAO);	
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glfwSwapBuffers(window);
	}
	Light.Delete();

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &quadVBO);
	glfwTerminate();
	return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true, 
	// и приложение после этого закроется
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else {
		camera.key_callback(key, action);
		key_callback_for_movement(key, action);
	}
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	camera.mouse_callback(xpos, ypos);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (keys[67]) {
		skaling += (yoffset * 0.1);
	} 
	else camera.scroll_callback(xoffset, yoffset);
}


void key_callback_for_movement(int key, int action) {
	if (action == 1) {
		keys[key] = true;
		std::cout << key;
	}
	else if (action == 0)
		keys[key] = false;
};