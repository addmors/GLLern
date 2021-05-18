#include "Shad.h"

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath)
{
	// 1. Получаем исходный код шейдера из filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// Удостоверимся, что ifstream объекты могут выкидывать исключения
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	try
	{
		// Открываем файлы
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// Считываем данные в потоки
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// Закрываем файлы
		vShaderFile.close();
		fShaderFile.close();
		// Преобразовываем потоки в массив GLchar
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// Если есть ошибки - вывести их
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILEDddddd\n" << infoLog << std::endl;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
void Shader::SetVec3(const GLchar* nameuniform, GLfloat x, GLfloat y, GLfloat z) {
	GLint Vec3Loc = glGetUniformLocation(ID, nameuniform);
	glUniform3f(Vec3Loc, x, y, z);
}
void Shader::SetVec3(const GLchar* nameuniform,glm::vec3 vec) {
	GLint Vec3Loc = glGetUniformLocation(ID, nameuniform);
	glUniform3f(Vec3Loc, vec.x, vec.y, vec.z);
}
void Shader::SetFloat(const GLchar* nameuniform, GLfloat flt) {
	GLint FloatLoc = glGetUniformLocation(ID, nameuniform);
	glUniform1f(FloatLoc, flt);
}
void Shader::SetMat4(const GLchar* nameuniform, glm::mat4 mat4) {
	GLint Mat4Loc = glGetUniformLocation(ID, nameuniform);
	glUniformMatrix4fv(Mat4Loc, 1, GL_FALSE, glm::value_ptr(mat4));
}
void Shader::SetInt(const GLchar* nameuniform, GLint num) {
	glUniform1i(glGetUniformLocation(ID, nameuniform), num);
}
void Shader::SetVec3(std::string* nameuniform, GLfloat x, GLfloat y, GLfloat z) {
	const char* name = nameuniform->c_str();
	GLint Vec3Loc = glGetUniformLocation(ID, name);
	glUniform3f(Vec3Loc, x, y, z);
}
void Shader::SetVec3(std::string* nameuniform, glm::vec3 vec) {
	const char* name = nameuniform->c_str();
	GLint Vec3Loc = glGetUniformLocation(ID, name);
	glUniform3f(Vec3Loc, vec.x, vec.y, vec.z);
}
void Shader::SetFloat(std::string* nameuniform, GLfloat flt) {
	const char* name = nameuniform->c_str();
	GLint FloatLoc = glGetUniformLocation(ID, name);
	glUniform1f(FloatLoc, flt);
}
void Shader::SetMat4(std::string* nameuniform, glm::mat4 mat4) {
	const char* name = nameuniform->c_str();
	GLint Mat4Loc = glGetUniformLocation(ID, name);
	glUniformMatrix4fv(Mat4Loc, 1, GL_FALSE, glm::value_ptr(mat4));
}
void Shader::SetInt(std::string* nameuniform, GLint num) {
	const char* name = nameuniform->c_str();
	glUniform1i(glGetUniformLocation(ID, name), num);
}

void Shader::Design(glm::mat4 view, std::vector<glm::vec3> &lightPos)
{

	std::string Position = "pointLights[].position";
	std::string Ambient = "pointLights[].ambient";
	std::string Diffuse = "pointLights[].diffuse";
	std::string Specular = "pointLights[].specular";
	std::string Constant = "pointLights[].constant";
	std::string Linear = "pointLights[].lenear";
	std::string Quadratic = "pointLights[].quadratic";
		
	////Фонарz
		SetVec3("spotlight.position", 0,0,0);
		SetVec3("spotlight.direction", 0,0,-1);
		SetFloat("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
		SetFloat("spotlight.outercutOff", glm::cos(glm::radians(17.5f)));
		SetVec3("spotlight.ambient", 0.0f, 0.0f, 0.0f);
		SetVec3("spotlight.diffuse", 1.0f, 1.0f, 1.0f); // darken the light a bit to fit the scene
		SetVec3("spotlight.specular", 1.0f, 1.0f, 1.0f);
		SetFloat("spotlight.constant", 1.0f);
		SetFloat("spotlight.linear", 0.027f);
		SetFloat("spotlight.quadratic", 0.0028f);
		//////Тут больше не фонарик )))
		////// Тут что то типо солнца, направленый свет без привязки к точке
		SetVec3("dirLight.direction", glm::mat3(transpose(inverse(view)))*glm::vec3(0.0f, -1.0f, 0.0f));
		SetVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		SetVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f); // darken the light a bit to fit the scene
		SetVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		//// Тут точечных 4 света(Вo все стороны)
		//
		for (GLuint i = 0; i < 4; i++) {
			
			Position.insert(12, std::to_string(i));
			Ambient.insert(12, std::to_string(i));
			Diffuse.insert(12, std::to_string(i));
			Specular.insert(12, std::to_string(i));
			Constant.insert(12, std::to_string(i));
			Linear.insert(12, std::to_string(i));
			Quadratic.insert(12, std::to_string(i));
			SetVec3(&Position, glm::vec3(view*glm::vec4(lightPos[i],1.0f)));
			SetVec3(&Ambient, 0.05f, 0.05f, 0.05f);
			SetVec3(&Diffuse, .8f, .8f, .8f);
			SetVec3(&Specular, 1.0f, 1.0f, 1.0f);
			SetFloat(&Constant, 1.0f);
			SetFloat(&Linear, 0.09);
			SetFloat(&Quadratic, 0.032);
			Position.erase(12, 1);
			Ambient.erase(12, 1);
			Diffuse.erase(12, 1);
			Specular.erase(12, 1);
			Constant.erase(12, 1);
			Linear.erase(12, 1);
			Quadratic.erase(12, 1);
		};
}
void Shader::Use() { glUseProgram(ID); }

