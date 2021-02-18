#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include<vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>; // Подключаем glew для того, чтобы получить все необходимые заголовочные файлы OpenGL

class Shader
{
public:
	unsigned int ID;
	// Идентификатор программы
	GLuint Program;
	// Конструктор считывает и собирает шейдер
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// Использование программы
	void Use();
	void SetVec3(const GLchar* nameuniform, GLfloat x, GLfloat y, GLfloat z);
	void SetVec3(const GLchar* nameuniform, glm::vec3 vec);
	void SetFloat(const GLchar* nameuniform, GLfloat flt);
	void SetMat4(const GLchar* nameuniform, glm::mat4 mat4);
	void SetInt(const GLchar* nameuniform, GLint num);

	void SetVec3(std::string* nameuniform, GLfloat x, GLfloat y, GLfloat z);
	void SetVec3(std::string* nameuniform, glm::vec3 vec);
	void SetFloat(std::string* nameuniform, GLfloat flt);
	void SetMat4(std::string* nameuniform, glm::mat4 mat4);
	void SetInt(std::string* nameuniform, GLint num);
	void Design(glm::mat4 view, std::vector<glm::vec3> &lightPos);
};

#endif