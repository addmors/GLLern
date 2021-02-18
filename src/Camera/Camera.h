#ifndef CAMERA
#define CAMERA

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>; // ���������� glew ��� ����, ����� �������� ��� ����������� ������������ ����� OpenGL

class Camera
{
public:
	// ������������� ���������
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	GLfloat fov;
	GLfloat yaw;
	GLfloat pitch;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat deltaTime;
	bool keys[1024];

	// ����������� ��������� � �������� ������
	Camera(glm::vec3 Pos, glm::vec3 Front, glm::vec3 Up, GLfloat fov, GLfloat yaw, GLfloat pitch);
	// ������������� ���������
	void key_callback(int key, int action);
	void mouse_callback(double xpos, double ypos);
	void scroll_callback(double xoffset, double yoffset);
	glm::mat4 LoocAt();
	void do_movement();
};

#endif