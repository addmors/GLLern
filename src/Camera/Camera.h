#ifndef CAMERA
#define CAMERA
#include<vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>; // Подключаем glew для того, чтобы получить все необходимые заголовочные файлы OpenGL
#include "../Precompile.h"

class Camera
{
public:
	// Идентификатор программы
	glm::vec3 cameraPos;
	glm::vec3 objPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight_;
	GLfloat fov;
	GLfloat yaw;
	GLfloat pitch;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat deltaTime;
	glm::vec3 front;
	GLfloat lengthtoobj;
	std::vector<bool>* keys;
	glm::mat4 view;

	Camera(glm::vec3 Pos, glm::vec3 Front, glm::vec3 Up, GLfloat fov);
	void mouse_callback(double xpos, double ypos);
	void scroll_callback(double xoffset, double yoffset);
	glm::vec3 cameraRight();
	void LoocAt();
	void do_movement();
};

#endif