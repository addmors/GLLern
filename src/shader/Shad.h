#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include<vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h> // ���������� glew ��� ����, ����� �������� ��� ����������� ������������ ����� OpenGL


static void ADDFILE(const GLchar* Path);
static void DELETEFILE(const GLchar* path);
static std::vector<const GLchar*> paths;
static const char* incPaths[1] = { "/" };
class Shader
{
public:
	unsigned int ID;
	// ������������� ���������
	std::vector<GLint> m_shaderObj;
	// ����������� ��������� � �������� ������
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	Shader();
	void AddShader(const GLchar* Path, GLint Type);
	void Finelize(); 
	// ������������� ���������
	void Use();
	void SetVec3(const GLchar* nameuniform, GLfloat x, GLfloat y, GLfloat z);
	void SetVec3(const GLchar* nameuniform, glm::vec3 vec);
	void SetFloat(const GLchar* nameuniform, GLfloat flt);
	void SetMat4(const GLchar* nameuniform, glm::mat4 mat4);
	void SetInt(const GLchar* nameuniform, GLint num);

	void SetVec3(std::string* nameuniform, GLfloat x, GLfloat y, GLfloat z);
	void SetVec3(std::string* nameuniform, glm::vec3 vec);
	void SetVec4(const GLchar* nameuniform, glm::vec4 vec);
	void SetFloat(std::string* nameuniform, GLfloat flt);
	void SetMat4(std::string* nameuniform, glm::mat4 mat4);
	void SetInt(std::string* nameuniform, GLint num);
	void Design(glm::mat4 view, std::vector<glm::vec3> &lightPos);
};
