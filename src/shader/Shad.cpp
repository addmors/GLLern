#include "Shad.h"

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath, const GLchar* geometryPath)
{
	Init();
	AddShader(vertexPath, GL_VERTEX_SHADER);
	AddShader(fragmentPath, GL_FRAGMENT_SHADER);
	if (geometryPath != nullptr)
		AddShader(geometryPath, GL_GEOMETRY_SHADER);
	Finelize();
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* tessCsPath, const GLchar* tessEsPath, const GLchar* geometryPath) {
	Init();
	AddShader(vertexPath, GL_VERTEX_SHADER);
	AddShader(fragmentPath, GL_FRAGMENT_SHADER);
	if(geometryPath != nullptr)
		AddShader(geometryPath, GL_GEOMETRY_SHADER);
	AddShader(tessCsPath, GL_TESS_CONTROL_SHADER);
	AddShader(tessEsPath, GL_TESS_EVALUATION_SHADER);
	Finelize();
}

void Shader::Init()
{
	ID = glCreateProgram();

	if (ID == 0) {
		fprintf(stderr, "Error creating shader program\n");
	}
}



void Shader::AddShader(const GLchar* path, GLint type) {
	GLuint ShaderObj = glCreateShader(type);
	std::string pathCode;
	std::ifstream pathShaderFile;
	pathShaderFile.exceptions(std::ifstream::badbit);
	try {
		pathShaderFile.open(path);
		std::stringstream pathShaderStream;
		pathShaderStream << pathShaderFile.rdbuf();
		pathShaderFile.close();
		pathCode = pathShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const GLchar* pathShaderCode = pathCode.c_str();

	glShaderSource(ShaderObj, 1, &pathShaderCode, NULL);

	glCompileShaderIncludeARB(ShaderObj, 1, incPaths, NULL);

	GLint success;
	GLchar infoLog[512];

	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(ShaderObj, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::"<<path<<"::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	m_shaderObj.push_back(ShaderObj);
	glAttachShader(ID, ShaderObj);
}


void Shader::Finelize()
{
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
	}

	glValidateProgram(ID);
	glGetProgramiv(ID, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
	}

	// Delete the intermediate shader objects that have been added to the program
	for (std::vector<GLint>::iterator it = m_shaderObj.begin(); it != m_shaderObj.end(); it++)
	{
		glDeleteShader(*it);
	}

	m_shaderObj.clear();
}




void Shader::SetVec3(const GLchar* nameuniform, GLfloat x, GLfloat y, GLfloat z) {
	GLint Vec3Loc = glGetUniformLocation(ID, nameuniform);
	glUniform3f(Vec3Loc, x, y, z);
}
void Shader::SetVec3(const GLchar* nameuniform,glm::vec3 vec) {
	GLint Vec3Loc = glGetUniformLocation(ID, nameuniform);
	glUniform3f(Vec3Loc, vec.x, vec.y, vec.z);
}
void Shader::SetVec4(const GLchar* nameuniform, glm::vec4 vec) {
	GLint Vec3Loc = glGetUniformLocation(ID, nameuniform);
	glUniform4f(Vec3Loc, vec.x, vec.y, vec.z,vec.w);
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

void Shader::Design(glm::mat4 view, std::vector<glm::vec3> &lightPos, glm::vec3& cameraPos, glm::vec3& cameraDir)
{

	std::string Position = "pointLights[].position";
	std::string Ambient = "pointLights[].ambient";
	std::string Diffuse = "pointLights[].diffuse";
	std::string Specular = "pointLights[].specular";
	std::string Constant = "pointLights[].constant";
	std::string Linear = "pointLights[].lenear";
	std::string Quadratic = "pointLights[].quadratic";
		
	////Фонарz
		SetVec3("spotlight.position", cameraPos);
		SetVec3("spotlight.direction", cameraDir );
		SetFloat("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
		SetFloat("spotlight.outercutOff", glm::cos(glm::radians(17.5f)));
		SetVec3("spotlight.ambient", 0.0f, 0.0f, 0.0f);
		SetVec3("spotlight.diffuse", 1.0f, 1.0f, 1.0f); // darken the light a bit to fit the scene
		SetVec3("spotlight.specular", 1.0f, 1.0f, 1.0f);
		SetFloat("spotlight.constant", 1.0f);
		SetFloat("spotlight.linear", 0.014f);
		SetFloat("spotlight.quadratic", 0.00007f);
		//////Тут больше не фонарик )))
		////// Тут что то типо солнца, направленый свет без привязки к точке
		SetVec3("dirLight.direction", glm::mat3(transpose(inverse(glm::mat3(1))))*glm::vec3(0.0f, -1.0f, 0.0f));
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
			SetVec3(&Position, glm::vec3(glm::vec4(lightPos[i],1.0f)));
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




void FileLoader::ADDFILE(const std::string path)
{
	std::string pathCode;
	std::ifstream pathShaderFile;
	pathShaderFile.exceptions(std::ifstream::badbit);
	try {
		pathShaderFile.open(path);
		std::stringstream pathShaderStream;
		pathShaderStream << pathShaderFile.rdbuf();
		pathShaderFile.close();
		pathCode = pathShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const GLchar* pathShaderCode = pathCode.c_str();
	std::string temp = '/' + path;
	
	glNamedStringARB(GL_SHADER_INCLUDE_ARB, strlen(temp.c_str()), temp.c_str(), strlen(pathShaderCode), pathShaderCode);
	names.push_back(path);
	
};
void FileLoader::DELETEFILE() {
	for (auto name : names) {
		glDeleteNamedStringARB(strlen(name.c_str()), name.c_str());
	}
};

