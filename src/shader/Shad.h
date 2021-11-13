#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include<vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h> // Подключаем glew для того, чтобы получить все необходимые заголовочные файлы OpenGL


static std::vector<const GLchar*> paths;

static const char* incPaths[1] = { "/" };


class FileLoader {
public:
	static void ADDFILE(std::string);
	static void DELETEFILE();
	static std::vector<std::string> names;
};


class Shader
{
public:
	unsigned int ID;
	// Идентификатор программы
	std::vector<GLint> m_shaderObj;
	// Конструктор считывает и собирает шейдер
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath = nullptr);
	Shader(const GLchar*, const GLchar*, const GLchar*, const GLchar*, const GLchar* );
	void Init();
	void AddShader(const GLchar* Path, GLint Type);
	void Finelize(); 
	// Использование программы
	void Use();
	void SetVec3(const GLchar* nameuniform, GLfloat x, GLfloat y, GLfloat z);
	void SetVec3(const GLchar* nameuniform, glm::vec3 vec);
	void SetVec2(const GLchar* nameuniform, GLfloat x, GLfloat y);
	void SetVec2(const GLchar* nameuniform, glm::vec2 vec);

	void SetFloat(const GLchar* nameuniform, GLfloat flt);
	void SetMat4(const GLchar* nameuniform, glm::mat4 mat4);
	void SetInt(const GLchar* nameuniform, GLint num);

	void SetVec3(std::string* nameuniform, GLfloat x, GLfloat y, GLfloat z);
	void SetVec3(std::string* nameuniform, glm::vec3 vec);
	void SetVec4(const GLchar* nameuniform, glm::vec4 vec);
	void SetVec4(const GLchar* nameuniform, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void SetFloat(std::string* nameuniform, GLfloat flt);
	void SetMat4(std::string* nameuniform, glm::mat4 mat4);
	void SetInt(std::string* nameuniform, GLint num);
	void Design(glm::mat4 view, std::vector<glm::vec3>& lightPos, glm::vec3& cameraPos, glm::vec3& cameraDir);
};
